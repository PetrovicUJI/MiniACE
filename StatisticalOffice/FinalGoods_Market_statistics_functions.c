#include "../header.h"
#include "../my_library_header.h"

#include "../Final_Goods_Market_agent_header.h"


/* \fn: int Final_goods_market_send_statistics()
 
* \brief: Final goods market send statistics.
 
 
* \timing: Monthly, the last day of the month.

 * \condition:

 
 *\ fgm_send_statistics_message structure: <!-- (sold_quantity, price_level, market_turnover) -->
 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int Final_goods_market_send_statistics()
{
	double price_level = 0.0;
	
	if(MONTHLY_SOLD_QUANTITY > 0)
	price_level = MONTHLY_MARKET_TURNOVER/MONTHLY_SOLD_QUANTITY;
	
	add_fgm_send_statistics_message(MONTHLY_SOLD_QUANTITY, price_level, MONTHLY_MARKET_TURNOVER);
	
	MONTHLY_SOLD_QUANTITY = 0.0;
	MONTHLY_MARKET_TURNOVER = 0.0;
	

	return 0;
}

