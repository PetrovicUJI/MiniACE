#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"

/* \fn: int Firm_send_final_goods_to_the_market()
 
* \brief: Firm send final goods to the market.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
 *\ final_goods_shipping message structure: <!-- (seller_id, quantity, price) -->

* \authors: Marko Petrovic
* \history: 24.10.2017-Marko: First implementation.
*/
int Firm_send_final_goods_to_the_market()
{
	add_final_goods_shipping_message(ID, PRODUCTION, PRICE);

    return 0;
}


/* \fn: int Firm_receive_update_from_the_final_goods_market()
 
* \brief: Firm receive update from the final goods market.
 
  *\ message filter: a.id == m.seller_id
 
 *\ final_goods_market_transaction_update message structure: <!-- (seller_id, sold_quantity, revenue) -->
 
 
* \timing: Daily.
 * \condition:
 

* \authors: Marko Petrovic
* \history: 26.10.2017-Marko: First implementation.
*/
int Firm_receive_updates_from_the_final_goods_market()
{
	START_FINAL_GOODS_MARKET_TRANSACTION_UPDATE_MESSAGE_LOOP
	
		MONTHLY_SOLD_QUANTITY += final_goods_market_transaction_update_message->sold_quantity;
		INVENTORIES -= final_goods_market_transaction_update_message->sold_quantity;
		PAYMENT_ACCOUNT += final_goods_market_transaction_update_message->revenue;
		MONTHLY_REVENUE += final_goods_market_transaction_update_message->revenue;
		
    FINISH_FINAL_GOODS_MARKET_TRANSACTION_UPDATE_MESSAGE_LOOP
	

    return 0;
}

