#include "../header.h"
#include "../my_library_header.h"

#include "../Credit_ClearingHouse_agent_header.h"


/* \fn: int CCH_receive_info()
 
* \brief: CCH receive info.
 
 
 *\ cb_send_info_message structure: 
<!-- (price_level, inflation, cb_interest_rate, alpha_capital_requirement, inflation_target) -->
 
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int CCH_receive_info()
{
	START_CB_SEND_INFO_MESSAGE_LOOP
		
		CB_INTEREST_RATE = cb_send_info_message->cb_interest_rate;
		ALPHA = cb_send_info_message->alpha_capital_requirement;

	FINISH_CB_SEND_INFO_MESSAGE_LOOP
	

    return 0;
}
