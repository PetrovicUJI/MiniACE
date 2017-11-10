#include "../header.h"
#include "../my_library_header.h"

#include "../Government_agent_header.h"


/* \fn: int Government_send_payments_to_CB()
 
* \brief: Government update CB account.
 
 
* \timing: Daily

*\ gov_to_cb_account_update_message structure: 	<!-- (payment_account, delta_loan, interest) -->

 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int Government_send_payments_to_CB()
{
	double delta_loan = 0.0;
	
	//account daily interest rate
	double interest = CB_DEBT*(INTEREST_RATE_ON_CB_DEBT/240);
	
	if(interest < 0)
	printf("\n ERROR in function Government_send_payments_to_CB: interest = %2.5f\n ", interest);

	PAYMENT_ACCOUNT -= interest;
	
	MONTHLY_INTEREST_PAYMENTS += interest;
	YEARLY_INTEREST_PAYMENTS += interest;
	
	if(PAYMENT_ACCOUNT < 0)
	{
		delta_loan = fabs(PAYMENT_ACCOUNT);
		PAYMENT_ACCOUNT = 0;
		CB_DEBT += delta_loan;
		
		
		add_gov_to_cb_account_update_message(PAYMENT_ACCOUNT, delta_loan, interest);
	}
	else
	{
		if(PAYMENT_ACCOUNT >= CB_DEBT)
		{
			PAYMENT_ACCOUNT -= CB_DEBT;
			
			delta_loan = (-1)*CB_DEBT;

			CB_DEBT = 0.0;
			
			add_gov_to_cb_account_update_message(PAYMENT_ACCOUNT, delta_loan, interest);
		}
		else
		{
			delta_loan = (-1)*PAYMENT_ACCOUNT;

			CB_DEBT -= PAYMENT_ACCOUNT;
			PAYMENT_ACCOUNT = 0.0;
			
			add_gov_to_cb_account_update_message(PAYMENT_ACCOUNT, delta_loan, interest);
		}
	}

    return 0;
}