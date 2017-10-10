#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"


/* \fn: int Firm_plan_financing()
 
* \brief: Firm plan how to finance its total costs.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 10.10.2017-Marko: First implementation.
*/
int Firm_plan_financing()
{
	double expected_investment_costs = 0.0;
	double expected_labor_costs = 0.0;
	double expected_instalment_payments = 0.0;
	double expected_interest_payments = 0.0;
	double expected_profit = 0.0;
	
	expected_investment_costs = INVESTMENT_PLAN*PHYSICAL_CAPITAL_PRICE;
	
	expected_labor_costs = LABOR_REQUIREMENT*WAGE;
	
	for(int i = 0; i < LOANS.size; i++)
	{
		expected_instalment_payments += LOANS.array[i].remaining_amount;
		
		expected_interest_payments += LOANS.array[i].instalment*LOANS.array[i].monthly_interest;
	}
		
	expected_profit = EXPECTED_DEMAND*PRICE*(1-VAT_RATE) - expected_labor_costs - expected_instalment_payments - expected_interest_payments;
	
	if(expected_profit > 0)
	{
		// profit after dividend payments
		expected_profit = expected_profit*PROFIT_ACCUMULATION_RATE;
	
		// profit after capital tax payments
		expected_profit = expected_profit*(1-CAPITAL_TAX_RATE);
	}
	
	TOTAL_FINANCIAL_NEEDS = expected_investment_costs + CURRENT_LIABILITIES - PAYMENT_ACCOUNT - expected_profit;
	
	// if firms take new loans they will set new prices according to new unit costs, therefore additional costs of new loans will be taken into account.
	TOTAL_FINANCIAL_NEEDS = max(0, TOTAL_FINANCIAL_NEEDS);
	
    return 0;
}
