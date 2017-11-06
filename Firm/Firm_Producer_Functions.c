#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"


/* \fn: int Firm_count_days_in_bankruptcy()
 
* \brief: Firm count days in bankruptcy.
 
 
* \timing: Daily.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 06.11.2017-Marko: First implementation.
*/
int Firm_count_days_in_bankruptcy()
{
	REMAINING_DAYS_OF_BANKRUPTCY--;

    return 0;
}

/* \fn: int Firm_update_physical_capital_and_price()
 
* \brief: Firm update price.
 
 
* \timing: Daily.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 06.11.2017-Marko: First implementation.
*/
int Firm_update_physical_capital_and_price()
{
	
	// update physical capital stock
	for (int i=PHYSICAL_CAPITAL_STOCK.size-1; i>-1; i--)
	{		
		PHYSICAL_CAPITAL_STOCK.array[i].months_in_use++;
	}
	
	PRICE = PRICE_LEVEL*0.5;
	PRODUCTION = 0.0;

    return 0;
}



/* \fn: int Firm_calculate_financial_needs()
 
* \brief: Firm calculate financial needs.
 
 
* \timing: 
 * \condition:
 
* \authors: Marko Petrovic
* \history: 06.11.2017-Marko: First implementation.
*/
int Firm_calculate_financial_needs()
{
	EXTERNAL_FINANCIAL_NEEDS = 0.0;
	
	double non_current_liabilities = 0.0;
	
	for(int i = 0; i < LOANS.size; i++)
	{
		non_current_liabilities += LOANS.array[i].remaining_amount;
	}
	
	if(fabs(NON_CURRENT_LIABILITIES - non_current_liabilities) >= 0.1) 
	printf("\n ERROR in function Firm_calculate_financial_needs: fabs(NON_CURRENT_LIABILITIES - non_current_liabilities) = %2.5f\n ", fabs(NON_CURRENT_LIABILITIES - non_current_liabilities));
	
	EXTERNAL_FINANCIAL_NEEDS = NON_CURRENT_LIABILITIES + CURRENT_LIABILITIES - PAYMENT_ACCOUNT;

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
	PRODUCTION_PLAN = 0.0;
	
	for(int i = 0; i < SOLD_QUANTITIES_VECTOR.size; i++)
	{
		EXPECTED_DEMAND += SOLD_QUANTITIES_VECTOR.array[i];
	}
	
	if(SOLD_QUANTITIES_VECTOR.size > 0) {EXPECTED_DEMAND = 1.1*(EXPECTED_DEMAND/SOLD_QUANTITIES_VECTOR.size);}
	
	
	if(EXPECTED_DEMAND > INVENTORIES)
	PRODUCTION_PLAN = EXPECTED_DEMAND - INVENTORIES;

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
	if(PRODUCTIVITY <= 0) 
	printf("\n ERROR in function Firm_plan_labor_demand: PRODUCTIVITY = %2.5f\n ", PRODUCTIVITY);
	
	if(PHYSICAL_CAPITAL > 0) {CAPACITY_UTILIZATION = min(1,(PRODUCTION_PLAN/(PHYSICAL_CAPITAL*PRODUCTIVITY)));}
	else {CAPACITY_UTILIZATION = 0;}
	
	
	if(CAPITAL_LABOR_RATIO <= 0)
	printf("\n ERROR in function Firm_plan_labor_demand: CAPITAL_LABOR_RATIO = %2.5f\n ", CAPITAL_LABOR_RATIO);


	LABOR_REQUIREMENT = (int)ceil(CAPACITY_UTILIZATION*(PHYSICAL_CAPITAL/CAPITAL_LABOR_RATIO));
	
	// For a small production plan the number of employess is reduced by 50%. Minimum will be 1 worker.
	LABOR_REQUIREMENT = max(LABOR_REQUIREMENT, (int)ceil(EMPLOYEES.size*0.5));

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
	if(PHYSICAL_CAPITAL == 0)
	{
		INVESTMENT_PLAN = LABOR_REQUIREMENT*CAPITAL_LABOR_RATIO;
		return 0;
	}
	
	double profit_rate = 0.0;
	double capacity_growth_rate = 0.0;
	
	INVESTMENT_PLAN = 0.0;
	
	if(NON_CURRENT_ASSETS > 1) {profit_rate = OPERATING_CASH_FLOW/NON_CURRENT_ASSETS;}
	else {profit_rate = OPERATING_CASH_FLOW;}
	
	
	if(TARGET_PROFIT_RATE <= 0)
	printf("\n ERROR in function Firm_plan_investment: TARGET_PROFIT_RATE = %2.5f\n ", TARGET_PROFIT_RATE);

	if(TARGET_CAPACITY_UTILIZATION <= 0) 
	printf("\n ERROR in function Firm_plan_investment: TARGET_CAPACITY_UTILIZATION = %2.5f\n ", TARGET_CAPACITY_UTILIZATION);
	
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
	
	double profit_accumulation_rate = 1.0;
	double investment_plan = 0.0;
	
	for(int i = 0; i < LOANS.size; i++)
	{
		expected_instalment_payments += LOANS.array[i].instalment;
		
		expected_interest_payments += LOANS.array[i].instalment*LOANS.array[i].monthly_interest;
	}
	
	expected_labor_costs = LABOR_REQUIREMENT*WAGE;
	
	int denum = 20;
	int num = denum + 1;
	do
	{
		num--;
		
		investment_plan = INVESTMENT_PLAN * (num/denum);
		
		expected_investment_costs = investment_plan*PHYSICAL_CAPITAL_PRICE;
		
		expected_profit = EXPECTED_DEMAND*PRICE*(1-VAT_RATE) - expected_labor_costs - expected_instalment_payments - expected_interest_payments;
		
		if(expected_profit > 0)
		{
			// profit after dividend payments
			expected_profit = expected_profit*profit_accumulation_rate;
		
			// profit after capital tax payments
			expected_profit = expected_profit*(1-CAPITAL_TAX_RATE);
		}
		
		EXTERNAL_FINANCIAL_NEEDS = expected_investment_costs + CURRENT_LIABILITIES*SHORT_TERM_LOAN_REPAYMENT_TARGET - PAYMENT_ACCOUNT - expected_profit;
		
	}while(investment_plan >= 1 && EXTERNAL_FINANCIAL_NEEDS >= 1);
	
	INVESTMENT_PLAN = investment_plan;
	
	if(INVESTMENT_PLAN < 1)
	INVESTMENT_PLAN = 0;

	
	if(EXTERNAL_FINANCIAL_NEEDS < 0)
	EXTERNAL_FINANCIAL_NEEDS = 0;

	
	return 0;
}

/* \fn: int Firm_produce_final_goods()
 
* \brief: Firm produce final goods.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 17.10.2017-Marko: First implementation.
*/
int Firm_produce_final_goods()
{
	PRODUCTION = 0.0;
	
	if(PRODUCTION_PLAN > 0)
	PRODUCTION = min(CAPITAL_LABOR_RATIO*EMPLOYEES.size,PHYSICAL_CAPITAL)*PRODUCTIVITY;

	if(PRODUCTION < 0)
	printf("\n ERROR in function Firm_produce_final_goods: PRODUCTION = %2.5f\n ", PRODUCTION);
	
	// update physical capital stock
	for (int i=PHYSICAL_CAPITAL_STOCK.size-1; i>-1; i--)
	{
		//update after the production takes place and before new capital is bought: months_in_use++
		
		PHYSICAL_CAPITAL_STOCK.array[i].months_in_use++;
	}
	
    return 0;
}



/* \fn: int Firm_calculate_unit_costs_and_set_price()
 
* \brief: Firm use unit costs and markup rate to set new price.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 17.10.2017-Marko: First implementation.
*/
int Firm_calculate_unit_costs_and_set_price()
{
	double unit_costs_old = 0.0;
	double current_costs = 0.0;
	double total_interest_payment = 0.0;
	double total_instalment_payment = 0.0;
	double labor_cost = 0.0;
	
	
	for(int i = 0; i < LOANS.size; i++)
	{
		total_instalment_payment += LOANS.array[i].instalment;
		
		total_interest_payment += LOANS.array[i].instalment*LOANS.array[i].monthly_interest;
	}
	
	
	for(int i = 0; i < EMPLOYEES.size; i++)
	{
		labor_cost += EMPLOYEES.array[i].wage;
	}
	
	unit_costs_old = UNIT_COSTS;
	current_costs = labor_cost + PHYSICAL_CAPITAL_DEPRECIATION_COST + total_instalment_payment + total_interest_payment + EXTERNAL_FINANCIAL_NEEDS;
	UNIT_COSTS = (unit_costs_old*INVENTORIES + current_costs)/(INVENTORIES+PRODUCTION);
	
	PRICE_LAST_MONTH = PRICE;
	PRICE = UNIT_COSTS*(1 + MARK_UP)*(1 + VAT_RATE);
	PRICE = (1-CB_TRUST)*PRICE + CB_TRUST*(PRICE_LAST_MONTH * (INFLATION_TARGET/12 + 1));
	
	INVENTORIES += PRODUCTION;
	
	// inventories are estimated at average market price
	CURRENT_ASSETS = INVENTORIES*PRICE_LEVEL;
	
    return 0;
}



