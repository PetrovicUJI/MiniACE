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

