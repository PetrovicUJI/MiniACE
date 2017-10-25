#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"

/* \fn: int Firm_send_final_goods_to_the_market()
 
* \brief: Firm send final goods to the market.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
 *\ final good message structure: <!-- (seller_id, quantity, price) -->

* \authors: Marko Petrovic
* \history: 24.10.2017-Marko: First implementation.
*/
int Firm_send_final_goods_to_the_market()
{
	
	add_final_good_message(ID, PRODUCTION, PRICE);

    return 0;
}





