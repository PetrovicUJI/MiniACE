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

/* \fn: int Firm_account_interest_on_short_term_borrowing()
 
* \brief: Firm account interest on short term borrowing.
 
 
* \timing: Daily.
 * \condition:
 
 
* \authors: Marko Petrovic
* \history: 27.10.2017-Marko: First implementation.
*/
int Firm_account_interest_on_short_term_borrowing()
{
	SHORT_TERM_BORROWING_INTEREST_PAYMENT += CURRENT_LIABILITIES*(MONTHLY_INTEREST_RATE_ON_SHORT_TERM_BORROWING/20);

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



/* \fn: int Firm_update_capacity()
 
* \brief: Firm update capital stock.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 
 *\ physical_capital_batch data structure:
    <!-- id, months_in_use, capital_units, price, current_value, productivity  -->
 

* \authors: Marko Petrovic
* \history: 05.10.2017-Marko: First implementation.
*/
int Firm_update_capacity()
{	
	PHYSICAL_CAPITAL = 0.0;
	PHYSICAL_CAPITAL_DEPRECIATION_COST = 0.0;
	
	NON_CURRENT_ASSETS = 0.0;
	
	for (int i=PHYSICAL_CAPITAL_STOCK.size-1; i>-1; i--)
	{
		//update after the production takes place and before new capital is bouth: months_in_use++

		if(PHYSICAL_CAPITAL_STOCK.array[i].months_in_use >= PHYSICAL_CAPITAL_DURATION)
		{
			remove_Physical_capital_batch(&PHYSICAL_CAPITAL_STOCK,i);
		}
		else
		{			
			if(PHYSICAL_CAPITAL_DURATION <= 0) 
			printf("\n ERROR in function Firm_update_capacity: PHYSICAL_CAPITAL_DURATION = %2.5f\n ", PHYSICAL_CAPITAL_DURATION);
			
			PHYSICAL_CAPITAL_STOCK.array[i].current_value = PHYSICAL_CAPITAL_STOCK.array[i].capital_units*PHYSICAL_CAPITAL_STOCK.array[i].price*(1-(PHYSICAL_CAPITAL_STOCK.array[i].months_in_use/PHYSICAL_CAPITAL_DURATION));
			
			// Capital costs should be evaluated at the current price or capital price???
			
			//PHYSICAL_CAPITAL_DEPRECIATION_COST += (PHYSICAL_CAPITAL_STOCK.array[i].capital_units/PHYSICAL_CAPITAL_DURATION)*PHYSICAL_CAPITAL_PRICE;
			PHYSICAL_CAPITAL_DEPRECIATION_COST += (PHYSICAL_CAPITAL_STOCK.array[i].capital_units/PHYSICAL_CAPITAL_DURATION)*PHYSICAL_CAPITAL_STOCK.array[i].price;
			
			PHYSICAL_CAPITAL += PHYSICAL_CAPITAL_STOCK.array[i].capital_units;
			
			NON_CURRENT_ASSETS += PHYSICAL_CAPITAL_STOCK.array[i].current_value;
			
			if(PHYSICAL_CAPITAL < 0) 
			printf("\n ERROR in function Firm_update_capacity: PHYSICAL_CAPITAL = %2.5f\n ", PHYSICAL_CAPITAL);
		}
	}
	
	// inventories are estimated at average market price
	CURRENT_ASSETS = INVENTORIES*PRICE_LEVEL;

    return 0;
}



/* \fn: int Firm_compute_financial_payments()
 
* \brief: Firm compute financial payments.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 
*\ Loan data structure: 
<!-- (creditor_id, repayment_period_months, remaining_months, remaining_amount, 
instalment, monthly_interest, loan_value_at_risk, loan_instalment_value_at_risk) -->
 
* \authors: Marko Petrovic
* \history: 27.10.2017-Marko: First implementation.
*/
int Firm_compute_financial_payments()
{
	TOTAL_LOAN_INSTALMENT_PAYMENT = 0.0;
	TOTAL_LOAN_INTEREST_PAYMENT = 0.0;
	
	for(int i = 0; i < LOANS.size; i++)
	{
		TOTAL_LOAN_INSTALMENT_PAYMENT += LOANS.array[i].instalment;
		TOTAL_LOAN_INTEREST_PAYMENT += LOANS.array[i].instalment*LOANS.array[i].monthly_interest;
		
		LOANS.array[i].remaining_months--;
		LOANS.array[i].remaining_amount -= LOANS.array[i].instalment;
		LOANS.array[i].loan_value_at_risk -= LOANS.array[i].loan_instalment_value_at_risk;
		
		if(LOANS.array[i].remaining_months == 0)
		remove_Loan(&LOANS,i);
	}
	
	TOTAL_FINANCIAL_PAYMENT = 
	TOTAL_LOAN_INSTALMENT_PAYMENT+
	TOTAL_LOAN_INTEREST_PAYMENT+
	SHORT_TERM_BORROWING_INTEREST_PAYMENT;
	

    return 0;
}


/* \fn: int Firm_compute_income_statement()
 
* \brief: Firm compute income statement.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 

 
* \authors: Marko Petrovic
* \history: 27.10.2017-Marko: First implementation.
*/
int Firm_compute_income_statement()
{
	CAPITAL_TAX_PAYMENT = 0.0;

	if(MONTHLY_REVENUE < 0) 
	printf("\n ERROR in function Firm_compute_income_statement: MONTHLY_REVENUE = %2.5f\n ", MONTHLY_REVENUE);	

	VAT_PAYMENT = MONTHLY_REVENUE*VAT_RATE;
	
	EBT = MONTHLY_REVENUE - VAT_PAYMENT - MONTHLY_WAGE_PAYMENT - TOTAL_FINANCIAL_PAYMENT - PHYSICAL_CAPITAL_DEPRECIATION_COST;
	
	
	/* DIVIDEND PAYMENT DECISION!!!*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		if(PAYMENT_ACCOUNT - VAT_PAYMENT - TOTAL_FINANCIAL_PAYMENT - CURRENT_LIABILITIES < 0 || EBT < 0)
		{
			PROFIT_ACCUMULATION_RATE = 1;
		}
		else
		{
			PROFIT_ACCUMULATION_RATE = 0.2;
		}
		
		DIVIDEND_PAYMENT = EBT*(1-PROFIT_ACCUMULATION_RATE);
		
		DIVIDEND_PAYMENT = max(0, PAYMENT_ACCOUNT - VAT_PAYMENT - TOTAL_FINANCIAL_PAYMENT - CURRENT_LIABILITIES - DIVIDEND_PAYMENT);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	EBT -= DIVIDEND_PAYMENT;
	
	if(EBT > 0)
	CAPITAL_TAX_PAYMENT = EBT*CAPITAL_TAX_RATE;
	
	NET_EARNINGS = EBT - CAPITAL_TAX_PAYMENT;

    return 0;
}

/* \fn: int Firm_pay_dividends()
 
* \brief: Firm pay dividends.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 
*\ dividend_payment message structure: 	<!-- (stockholder_id, dividend) -->
*\ message filter: a.id == m.stockholder_id


*\ Stockholder data structure:  <!-- (stockholder_id, shares) -->
 

* \authors: Marko Petrovic
* \history: 27.10.2017-Marko: First implementation.
*/
int Firm_pay_dividends()
{
	if(DIVIDEND_PAYMENT == 0)
	return 0;
	
	int total_number_of_shares = 0;
	int stockholder_id = 0;
	double dividend = 0.0;
	
	for(int i = 0; i < STOCKHOLDERS_LIST.size; i++)
	{
		total_number_of_shares += STOCKHOLDERS_LIST.array[i].shares;
	}
	
	for(int i = 0; i < STOCKHOLDERS_LIST.size; i++)
	{
		stockholder_id = STOCKHOLDERS_LIST.array[i].stockholder_id;
		dividend = (DIVIDEND_PAYMENT/total_number_of_shares)*STOCKHOLDERS_LIST.array[i].shares;
		
		add_dividend_payment_message(stockholder_id, dividend);
	}
	
	PAYMENT_ACCOUNT -= DIVIDEND_PAYMENT;


    return 0;
}

/* \fn: int Firm_pay_financial_expenses()
 
* \brief: Firm pay financial expenses.
 
* \timing: Monthly on the last activation day.
 * \condition:
 

* \authors: Marko Petrovic
* \history: 27.10.2017-Marko: First implementation.
*/
int Firm_pay_financial_expenses()
{
	double expected_payment_account = PAYMENT_ACCOUNT - VAT_PAYMENT - TOTAL_FINANCIAL_PAYMENT - CAPITAL_TAX_PAYMENT;
	double expected_current_liabilities = CURRENT_LIABILITIES;
	double expected_equity = 0.0;

	if(expected_payment_account < 0)
	{
		expected_current_liabilities += fabs(expected_payment_account);
		
		expected_equity = CURRENT_ASSETS + NON_CURRENT_ASSETS - NON_CURRENT_LIABILITIES - expected_current_liabilities;
		
		if(expected_equity < 0)
		{
			ACTIVE = 0;
			return 0;
		}
	}
	
	// execute financial payment here and calculate balance sheet later!!!

	
    return 0;
}

/* \fn: int Firm_compute_balance_sheet()
 
* \brief: Firm compute balance sheet.
 
* \timing: Monthly on the last activation day.
 * \condition:
 

* \authors: Marko Petrovic
* \history: 27.10.2017-Marko: First implementation.
*/
int Firm_compute_balance_sheet()
{
	
	
    return 0;
}
