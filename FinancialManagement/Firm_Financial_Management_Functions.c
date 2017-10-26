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
		expected_instalment_payments += LOANS.array[i].instalment;
		
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
	
	EXTERNAL_FINANCIAL_NEEDS = expected_investment_costs + CURRENT_LIABILITIES - PAYMENT_ACCOUNT - expected_profit;
	
	// if firms take new loans they will set new prices according to new unit costs, therefore additional costs of new loans will be taken into account.
	EXTERNAL_FINANCIAL_NEEDS = max(0, EXTERNAL_FINANCIAL_NEEDS);
	
    return 0;
}

/* \fn: int Firm_account_and_pay_salaries()
 
* \brief: Firm account and pay salaries to eployees.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 
 *\ wage_payment_message structure: <!-- (employee_id, wage) -->
 
 *\ Employee data structure: 	<!-- id, wage -->
 
* \authors: Marko Petrovic
* \history: 26.10.2017-Marko: First implementation.
*/
int Firm_account_and_pay_salaries()
{
	MONTHLY_WAGE_PAYMENT = 0.0;
	
	for(int i = 0; i < EMPLOYEES.size; i++)
	{
		add_wage_payment_message(EMPLOYEES.array[i].id, EMPLOYEES.array[i].wage);
		
		MONTHLY_WAGE_PAYMENT += EMPLOYEES.array[i].wage;
	}
	
	PAYMENT_ACCOUNT -= MONTHLY_WAGE_PAYMENT;
	

    return 0;
}
