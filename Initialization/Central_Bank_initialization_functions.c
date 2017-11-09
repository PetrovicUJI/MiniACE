#include "../header.h"
#include "../my_library_header.h"

#include "../Central_Bank_agent_header.h"



/* \fn: int CB_initialization()
 
* \brief: CB send ID.
 
 *\ init_cb_to_gov_message structure: 	<!-- (cb_id) -->
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int CB_initialization()
{
	
	add_init_cb_to_gov_message(ID);
	

    return 0;
}

/* \fn: int CB_receive_initialization()
 
* \brief: CB receive initialization and set initialization to zero.

*\gov_init_balance_sheets message structure:
*\ <!-- (agent_id, current_assets, non_current_assets, payment_account, current_liabilities, non_current_liabilities) -->
 filter: a.id == m.agent_id
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int CB_receive_initialization()
{
	
	START_GOV_INIT_BALANCE_SHEETS_MESSAGE_LOOP
	
		FIAT_MONEY_BANKS = gov_init_balance_sheets_message->current_assets;
		GOV_DEBT = gov_init_balance_sheets_message->non_current_assets;
		PAYMENT_ACCOUNT = gov_init_balance_sheets_message->payment_account;
		DEPOSITS = gov_init_balance_sheets_message->current_liabilities;
		FIAT_MONEY = gov_init_balance_sheets_message->non_current_liabilities;
	
    FINISH_GOV_INIT_BALANCE_SHEETS_MESSAGE_LOOP
	
	
	TOTAL_ASSETS = FIAT_MONEY_BANKS + GOV_DEBT + PAYMENT_ACCOUNT;
	TOTAL_LIABILITIES = DEPOSITS + FIAT_MONEY;
	EQUITY = TOTAL_ASSETS - TOTAL_LIABILITIES;
	
	
	INITIALIZATION = 0;

    return 0;
}