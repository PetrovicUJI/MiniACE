#include "../header.h"
#include "../my_library_header.h"

#include "../Bank_agent_header.h"


/* \fn: int Bank_send_payments_to_CB()
 
* \brief: Bank update CB account.
 
 
* \timing: Daily

*\ bank_to_cb_account_update_message structure: 	<!-- (payment_account, delta_loan, interest) -->

 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int Bank_send_payments_to_CB()
{
	double delta_loan = 0.0;
	
	//account daily interest rate
	double interest = CB_DEBT*(INTEREST_RATE_ON_CB_DEBT/240);
	
	if(interest < 0)
	printf("\n ERROR in function Government_send_payments_to_CB: interest = %2.5f\n ", interest);

	PAYMENT_ACCOUNT -= interest;
	EQUITY -= interest;
	MONTHLY_PROFIT -= interest;
	
	
	if(PAYMENT_ACCOUNT < 0)
	{
		delta_loan = fabs(PAYMENT_ACCOUNT);
		PAYMENT_ACCOUNT = 0;
		CB_DEBT += delta_loan;
		
		
		add_bank_to_cb_account_update_message(PAYMENT_ACCOUNT, delta_loan, interest);
	}
	else
	{
		if(PAYMENT_ACCOUNT >= CB_DEBT)
		{
			PAYMENT_ACCOUNT -= CB_DEBT;
			
			delta_loan = (-1)*CB_DEBT;

			CB_DEBT = 0.0;
			
			add_bank_to_cb_account_update_message(PAYMENT_ACCOUNT, delta_loan, interest);
		}
		else
		{
			delta_loan = (-1)*PAYMENT_ACCOUNT;

			CB_DEBT -= PAYMENT_ACCOUNT;
			PAYMENT_ACCOUNT = 0.0;
			
			add_bank_to_cb_account_update_message(PAYMENT_ACCOUNT, delta_loan, interest);
		}
	}
	
	
	TOTAL_ASSETS = CURRENT_ASSETS + NON_CURRENT_ASSETS + PAYMENT_ACCOUNT;
	TOTAL_LIABILITIES = DEPOSITS + CB_DEBT;
	
	if(fabs(EQUITY + TOTAL_LIABILITIES - TOTAL_ASSETS) > 0.1)
	{
		printf("\n ERROR in function CB_receive_account_update: Balance sheet is not in balance = %2.5f\n ");
		printf("\n EQUITY = %2.5f\n ", EQUITY);
		printf("\n TOTAL_LIABILITIES = %2.5f\n ", TOTAL_LIABILITIES);
		printf("\n TOTAL_ASSETS = %2.5f\n ", TOTAL_ASSETS);
	}
	
	

    return 0;
}


