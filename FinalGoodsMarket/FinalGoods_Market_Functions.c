#include "../header.h"
#include "../my_library_header.h"

#include "../Final_Goods_Market_agent_header.h"
#include "FinalGoods_Market_AUX_Headers.h"

/* \fn: int Final_goods_market_receive_products()
 
* \brief: Final goods market receive products.
 
 
* \timing: Daily
* \timing: 
 * \condition:
 
 *\ final_goods_shipping message structure: <!-- (seller_id, quantity, price) -->
 *\ data type Final_good structure: <!-- (seller_id, quantity, price, sold_quantity) -->
 
 *\ final_goods_order_message structure	  		<!-- (buyer_id, budget) -->
 *\  data type Final_goods_order structure      <!-- (buyer_id, budget) -->
 
* \authors: Marko Petrovic
* \history: 23.10.2017-Marko: First implementation.
*/
int Final_goods_market_receive_products_and_orders()
{
	int seller_id = 0;
	double quantity = 0.0;
	double price = 0.0;
	
	START_FINAL_GOODS_SHIPPING_MESSAGE_LOOP
	
		seller_id = final_goods_shipping_message->seller_id;
		quantity = final_goods_shipping_message->quantity;
		price = final_goods_shipping_message->price;
		
		int not_present = 1;
		for(int i = 0; i < FINAL_GOODS_LIST.size; i++)
		{
			if (FINAL_GOODS_LIST.array[i].seller_id == seller_id)
			{
				FINAL_GOODS_LIST.array[i].quantity += quantity;
				FINAL_GOODS_LIST.array[i].price = price;
				not_present = 0;
				//break;
			}
		}
		
		if(not_present)
		add_Final_good(&FINAL_GOODS_LIST, seller_id, quantity, price, 0);
	
    FINISH_FINAL_GOODS_SHIPPING_MESSAGE_LOOP
	
	
	int buyer_id = 0;
	double budget = 0.0;
	
	/* Make sure that the order array is clear.*/
    if (FINAL_GOODS_ORDERS_LIST.size > 0) 
	reset_Final_goods_order_array(&FINAL_GOODS_ORDERS_LIST);

	START_FINAL_GOODS_ORDER_MESSAGE_LOOP
	
		buyer_id = final_goods_order_message->buyer_id;
		budget = final_goods_order_message->budget;
		
		add_Final_goods_order(&FINAL_GOODS_ORDERS_LIST, buyer_id, budget);
		
    FINISH_FINAL_GOODS_ORDER_MESSAGE_LOOP

	
    return 0;
}


/* \fn: int Final_goods_market_send_products_to_buyers()
 
* \brief: Final goods market send products.
 
 
* \timing: Daily
* \timing: 
 * \condition:
 
 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Final_goods_market_send_products_to_buyers()
{

	
    return 0;
}