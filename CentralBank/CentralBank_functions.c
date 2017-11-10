#include "../header.h"
#include "../my_library_header.h"

#include "../Central_Bank_agent_header.h"


/* \fn: int CB_receive_account_update()
 
* \brief: Central Bank receive account update.
 
 
* \timing: Daily

*\ gov_to_cb_account_update_message structure: 	<!-- (payment_account, delta_loan, interest) -->

*\ bank_to_cb_account_update_message structure: 	<!-- (payment_account, delta_loan, interest) -->

 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int CB_receive_account_update()
{
	DEPOSITS = 0;
	PAYMENT_ACCOUNT = 0;
	
	START_GOV_TO_CB_ACCOUNT_UPDATE_MESSAGE_LOOP
	
		DEPOSITS += gov_to_cb_account_update_message->payment_account;
		PAYMENT_ACCOUNT += gov_to_cb_account_update_message->payment_account;
		
		GOV_DEBT += gov_to_cb_account_update_message->delta_loan;
		FIAT_MONEY += gov_to_cb_account_update_message->delta_loan;
		
		EQUITY += gov_to_cb_account_update_message->interest;
		PAYMENT_ACCOUNT += gov_to_cb_account_update_message->interest;
		
    FINISH_GOV_TO_CB_ACCOUNT_UPDATE_MESSAGE_LOOP
	
	
	START_BANK_TO_CB_ACCOUNT_UPDATE_MESSAGE_LOOP
	
		DEPOSITS += bank_to_cb_account_update_message->payment_account;
		PAYMENT_ACCOUNT += bank_to_cb_account_update_message->payment_account;
		
		FIAT_MONEY_BANKS += bank_to_cb_account_update_message->delta_loan;
		FIAT_MONEY += bank_to_cb_account_update_message->delta_loan;
		
		EQUITY += bank_to_cb_account_update_message->interest;
		PAYMENT_ACCOUNT += bank_to_cb_account_update_message->interest;
		
    FINISH_BANK_TO_CB_ACCOUNT_UPDATE_MESSAGE_LOOP
	
	// money distruction
	
	PAYMENT_ACCOUNT -= EQUITY;
	EQUITY = 0.0;
	
	TOTAL_ASSETS = FIAT_MONEY_BANKS + GOV_DEBT + PAYMENT_ACCOUNT;
	TOTAL_LIABILITIES = FIAT_MONEY + DEPOSITS;
	
	if(fabs(EQUITY + TOTAL_LIABILITIES - TOTAL_ASSETS) > 0.1)
	{
		printf("\n ERROR in function CB_receive_account_update: Balance sheet is not in balance = %2.5f\n ");
		printf("\n EQUITY = %2.5f\n ", EQUITY);
		printf("\n TOTAL_LIABILITIES = %2.5f\n ", TOTAL_LIABILITIES);
		printf("\n TOTAL_ASSETS = %2.5f\n ", TOTAL_ASSETS);
	}

    return 0;
}

