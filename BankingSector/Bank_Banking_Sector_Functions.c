#include "../header.h"
#include "../my_library_header.h"

#include "../Bank_agent_header.h"


/* \fn: int Bank_pay_tax_and_dividends()
 
* \brief: Bank pay tax and dividends.
 
 
* \timing: The last day of the month.
 * \condition:

*\ dividend_payment message structure: <!-- (firm_id, dividend) -->

*\ tax_payments_message structure:
<!-- (gov_id, tax_payment) -->
 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int Bank_pay_tax_and_dividends()
{
	if(OUTSTANDING_SHARES == 0)
	printf("\n ERROR in function  Bank_pay_tax_and_dividends: OUTSTANDING_SHARES = %2.5f\n ", OUTSTANDING_SHARES);
	
	if(DIVIDEND_PAYMENT > 0)
	{
		double dividend = 0.0;

		dividend = (DIVIDEND_PAYMENT/OUTSTANDING_SHARES);
		
		add_dividend_payment_message(ID, dividend);

		PAYMENT_ACCOUNT -= DIVIDEND_PAYMENT;
		EQUITY -= DIVIDEND_PAYMENT;
	}
	
	if(CAPITAL_TAX_PAYMENT > 0)
	{
		add_tax_payments_message(GOV_ID, CAPITAL_TAX_PAYMENT);
		
		PAYMENT_ACCOUNT -= CAPITAL_TAX_PAYMENT;
		EQUITY -= CAPITAL_TAX_PAYMENT;
	}
	
	MONTHLY_PROFIT = 0.0;

    return 0;
}












/* \fn: int Bank_offer_loans()
 
* \brief: Banks offer loans.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:

 *\ loan offer message structure: (agent_id, equity, value_at_risk, alpha)
 
* \authors: Marko Petrovic
* \history: 11.10.2017-Marko: First implementation.
*/
int Bank_offer_loans()
{
	if (VALUE_AT_RISK <= ALPHA*EQUITY)
	{
		add_loan_offer_message(ID, EQUITY, VALUE_AT_RISK, ALPHA);	
	}

    return 0;
}

/* \fn: int Bank_give_loans()
 
* \brief: Bank give loans.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:

 *\ new_loan_given_message structure: <!-- (creditor_id, credit_allowed, loan_value_at_risk) -->
 
 *\ message filter: a.id == m.creditor_id
 
* \authors: Marko Petrovic
* \history: 11.10.2017-Marko: First implementation.
*/
int Bank_give_loans()
{
	START_NEW_LOAN_GIVEN_MESSAGE_LOOP
		NON_CURRENT_ASSETS += new_loan_given_message->credit_allowed;
		DEPOSITS += new_loan_given_message->credit_allowed;
		VALUE_AT_RISK += new_loan_given_message->loan_value_at_risk;
    FINISH_NEW_LOAN_GIVEN_MESSAGE_LOOP
	

    return 0;
}

/* \fn: int Bank_receive_loan_repayments()
 
* \brief: Bank receive loan repayments.
 
 
* \timing: Daily.
 * \condition:

 *\ loan_repayment_message structure: 
<!-- (creditor_id, instalment_payments, interest_payments, loan_instalment_value_at_risk) -->


 *\ message filter: a.id == m.creditor_id
 
* \authors: Marko Petrovic
* \history: 28.10.2017-Marko: First implementation.
*/
int Bank_receive_loan_repayments()
{
	START_LOAN_REPAYMENT_MESSAGE_LOOP
	
		NON_CURRENT_ASSETS -= loan_repayment_message->instalment_payments;
		VALUE_AT_RISK -= loan_repayment_message->loan_instalment_value_at_risk;
		
		EQUITY += loan_repayment_message->interest_payments;
		PAYMENT_ACCOUNT += loan_repayment_message->interest_payments;
		MONTHLY_PROFIT += loan_repayment_message->interest_payments;
		
    FINISH_LOAN_REPAYMENT_MESSAGE_LOOP
	

    return 0;
}

/* \fn: int Bank_receive_account_update()
 
* \brief: Bank_receive account update.
 
 
* \timing: Daily.
 * \condition:

 *\ bank_account_update_message structure: 
 <!-- (bank_id, payment_account, delta_loan, delta_value_at_risk) -->
 

 *\ message filter: a.id == m.bank_id
 
* \authors: Marko Petrovic
* \history: 28.10.2017-Marko: First implementation.
*/
int Bank_receive_account_update()
{
	DEPOSITS = 0.0;
	
	START_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP
	
		CURRENT_ASSETS += bank_account_update_message->delta_loan;
		VALUE_AT_RISK += bank_account_update_message->delta_value_at_risk;
		
		DEPOSITS += bank_account_update_message->payment_account;
		
    FINISH_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP
	

    return 0;
}

/* \fn: int Bank_determine_dividends()
 
* \brief: Bank determine dividends.
 
 
* \timing: The last day of the month.
 * \condition:


 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int Bank_determine_dividends()
{
	DIVIDEND_PAYMENT = 0.0;
	
	if(MONTHLY_PROFIT == 0)
	return 0;

	if(MONTHLY_PROFIT < 0)
	{
		printf("\n ERROR in function Bank_determine_dividends: MONTHLY_PROFIT = %2.5f\n ", MONTHLY_PROFIT);
		return 0;
	}

	DIVIDEND_PAYMENT = 0.5*MONTHLY_PROFIT;
	
	CAPITAL_TAX_PAYMENT = (MONTHLY_PROFIT-DIVIDEND_PAYMENT)*CAPITAL_TAX_RATE;

    return 0;
}



