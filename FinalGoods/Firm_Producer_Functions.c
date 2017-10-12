#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"


/* \fn: int Firm_update_capacity()
 
* \brief: Firm update capital stock.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 05.10.2017-Marko: First implementation.
*/
int Firm_update_capacity()
{
	PHYSICAL_CAPITAL = 0.0;
	
	for (int i=PHYSICAL_CAPITAL_STOCK.size-1; i>-1; i--)
	{
		//update after the production takes place and before new capital is bouth
		//PHYSICAL_CAPITAL_STOCK.array[i].months_in_use++;

		if(PHYSICAL_CAPITAL_STOCK.array[i].months_in_use >= PHYSICAL_CAPITAL_DURATION)
		{
			remove_physical_capital_batch(&PHYSICAL_CAPITAL_STOCK,i);
		}
		else
		{
			
			if(PHYSICAL_CAPITAL_DURATION <= 0) {printf("\n ERROR in function Firm_update_capacity: PHYSICAL_CAPITAL_DURATION = %2.5f\n ", PHYSICAL_CAPITAL_DURATION);}
			
			PHYSICAL_CAPITAL_STOCK.array[i].current_value = PHYSICAL_CAPITAL_STOCK.array[i].capital_units*PHYSICAL_CAPITAL_STOCK.array[i].price*(1-(PHYSICAL_CAPITAL_STOCK.array[i].months_in_use/PHYSICAL_CAPITAL_DURATION));
			
			
			PHYSICAL_CAPITAL += PHYSICAL_CAPITAL_STOCK.array[i].capital_units;
			
			if(PHYSICAL_CAPITAL < 0) {printf("\n ERROR in function Firm_update_capacity: PHYSICAL_CAPITAL = %2.5f\n ", PHYSICAL_CAPITAL);}
		}
	}

    return 0;
}




/* \fn: int Firm_plan_production_quantity()
 
* \brief: Firm determine production plan.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 27.09.2017-Marko: First implementation.
*/
int Firm_plan_production_quantity()
{
	EXPECTED_DEMAND = 0.0;
	
	for(int i = 0; i < SOLD_QUANTITIES_VECTOR.size; i++)
	{
		EXPECTED_DEMAND += SOLD_QUANTITIES_VECTOR.array[i];
	}
	
	if(SOLD_QUANTITIES_VECTOR.size > 0) {EXPECTED_DEMAND = 1.1*(EXPECTED_DEMAND/SOLD_QUANTITIES_VECTOR.size);}
	
	
	if(EXPECTED_DEMAND < INVENTORIES) {PRODUCTION_PLAN = 0;}
	else {PRODUCTION_PLAN = EXPECTED_DEMAND - INVENTORIES;}

    return 0;
}


/* \fn: int Firm_plan_labor_demand()
 
* \brief: Firm determine labor demand.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 04.10.2017-Marko: First implementation.
*/
int Firm_plan_labor_demand()
{
	if(PRODUCTIVITY <= 0) {printf("\n ERROR in function Firm_plan_labor_demand: PRODUCTIVITY = %2.5f\n ", PRODUCTIVITY);}
	
	if(PHYSICAL_CAPITAL > 0) {CAPACITY_UTILIZATION = min(1,(PRODUCTION_PLAN/(PHYSICAL_CAPITAL*PRODUCTIVITY)));}
	else {CAPACITY_UTILIZATION = 0;}
	
	
	if(CAPITAL_LABOR_RATIO <= 0) {printf("\n ERROR in function Firm_plan_labor_demand: CAPITAL_LABOR_RATIO = %2.5f\n ", CAPITAL_LABOR_RATIO);}

	LABOR_REQUIREMENT = (int)ceil(CAPACITY_UTILIZATION*(PHYSICAL_CAPITAL/CAPITAL_LABOR_RATIO));

    return 0;
}

/* \fn: int Firm_plan_investment()
 
* \brief: Firm plan investment in physical capital.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 05.10.2017-Marko: First implementation.
*/
int Firm_plan_investment()
{
	double profit_rate = 0.0;
	double capacity_growth_rate = 0.0;
	
	INVESTMENT_PLAN = 0.0;
	
	if(NON_CURRENT_ASSETS > 1) {profit_rate = OPERATING_CASH_FLOW/NON_CURRENT_ASSETS;}
	else {profit_rate = OPERATING_CASH_FLOW;}
	
	
	if(TARGET_PROFIT_RATE <= 0) {printf("\n ERROR in function Firm_plan_investment: TARGET_PROFIT_RATE = %2.5f\n ", TARGET_PROFIT_RATE);}
	if(TARGET_CAPACITY_UTILIZATION <= 0) {printf("\n ERROR in function Firm_plan_investment: TARGET_CAPACITY_UTILIZATION = %2.5f\n ", TARGET_CAPACITY_UTILIZATION);}
	
	capacity_growth_rate = PROFIT_RATE_WEIGHTS*((profit_rate-TARGET_PROFIT_RATE)/TARGET_PROFIT_RATE) + CAPACITY_UTILIZATION_WEIGHTS*((CAPACITY_UTILIZATION-TARGET_CAPACITY_UTILIZATION)/TARGET_CAPACITY_UTILIZATION);

	if(capacity_growth_rate > 0) {INVESTMENT_PLAN = capacity_growth_rate*PHYSICAL_CAPITAL;}

    return 0;
}



/* \fn: int Firm_adjust_investment_plan()
 
* \brief: Firm adjust production plan and input demand due to limited financial funds. Firms use maximum utilization rate.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 11.10.2017-Marko: First implementation.
*/
int Firm_adjust_investment_plan()
{
	
	double expected_profit = 0.0;
	double expected_investment_costs = 0.0;
	double expected_labor_costs = 0.0;
	double expected_instalment_payments = 0.0;
	double expected_interest_payments = 0.0;
	
	for(int i = 0; i < LOANS.size; i++)
	{
		expected_instalment_payments += LOANS.array[i].instalment;
		
		expected_interest_payments += LOANS.array[i].instalment*LOANS.array[i].monthly_interest;
	}
	
	expected_labor_costs = LABOR_REQUIREMENT*WAGE;
	
	PROFIT_ACCUMULATION_RATE = 1;
	INVESTMENT_PLAN++;
	do
	{
		INVESTMENT_PLAN--;
		
		expected_investment_costs = INVESTMENT_PLAN*PHYSICAL_CAPITAL_PRICE;
		
		expected_profit = EXPECTED_DEMAND*PRICE*(1-VAT_RATE) - expected_labor_costs - expected_instalment_payments - expected_interest_payments;
		
		if(expected_profit > 0)
		{
			// profit after dividend payments
			expected_profit = expected_profit*PROFIT_ACCUMULATION_RATE;
		
			// profit after capital tax payments
			expected_profit = expected_profit*(1-CAPITAL_TAX_RATE);
		}
		
		EXTERNAL_FINANCIAL_NEEDS = expected_investment_costs + CURRENT_LIABILITIES - PAYMENT_ACCOUNT - expected_profit;
		
	}while(INVESTMENT_PLAN > 0 && EXTERNAL_FINANCIAL_NEEDS > 1);
	
	if(INVESTMENT_PLAN < 1)
	{
		INVESTMENT_PLAN = 0;
	}
	
	return 0;
}


/* \fn: int Firm_compute_sale_statistics()
 
* \brief: Firm store data about a monthly sold quantity.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 27.09.2017-Marko: First implementation.
*/
int Firm_compute_sale_statistics()
{
	
	remove_double(&SOLD_QUANTITIES_VECTOR,0);
    add_double(&SOLD_QUANTITIES_VECTOR,MONTHLY_SOLD_QUANTITY);
	
	MONTHLY_SOLD_QUANTITY = 0;

    return 0;
}



