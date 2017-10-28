#include "../header.h"
#include "../my_library_header.h"

#include "../Bank_agent_header.h"

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



