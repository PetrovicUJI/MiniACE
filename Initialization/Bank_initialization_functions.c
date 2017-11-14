#include "../header.h"
#include "../my_library_header.h"

#include "../Bank_agent_header.h"



/* \fn: int Bank_initialization()
 
* \brief: Bank send initialization info to the government.
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
  *\ bank_to_gov_ini message structure: <!-- (bank_id, outstanding_shares) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Bank_initialization()
{
	add_bank_to_gov_ini_message(ID, OUTSTANDING_SHARES);

    return 0;
}


/* \fn: int Bank_receive_initialization()
 
* \brief: Bank receive initialization info to the government.
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
*\gov_init_balance_sheets message structure:
*\ <!-- (agent_id, current_assets, non_current_assets, payment_account, current_liabilities, non_current_liabilities) -->
 filter: a.id == m.agent_id
 
* \authors: Marko Petrovic
* \history: 09.11.2017-Marko: First implementation.
*/
int Bank_receive_initialization()
{
	
	START_GOV_INIT_BALANCE_SHEETS_MESSAGE_LOOP
	
		CURRENT_ASSETS = gov_init_balance_sheets_message->current_assets;
		NON_CURRENT_ASSETS = gov_init_balance_sheets_message->non_current_assets;
		PAYMENT_ACCOUNT = gov_init_balance_sheets_message->payment_account;
		DEPOSITS = gov_init_balance_sheets_message->current_liabilities;
		CB_DEBT = gov_init_balance_sheets_message->non_current_liabilities;
	
    FINISH_GOV_INIT_BALANCE_SHEETS_MESSAGE_LOOP
	
	
	TOTAL_ASSETS = CURRENT_ASSETS + NON_CURRENT_ASSETS + PAYMENT_ACCOUNT;
	TOTAL_LIABILITIES = DEPOSITS + CB_DEBT; 
	EQUITY = TOTAL_ASSETS - TOTAL_LIABILITIES;
	
	VALUE_AT_RISK = 0;

	
	INITIALIZATION = 0;

    return 0;
}


