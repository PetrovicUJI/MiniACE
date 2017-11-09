#include "../header.h"
#include "../my_library_header.h"

#include "../Final_Goods_Market_agent_header.h"


/* \fn: int Final_Goods_Market_initialization()
 
* \brief: Final_Goods_Market set initialization.
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 

 *\ data type Final_good structure: <!-- (seller_id, quantity, price, sold_quantity) -->
 
  *\ final_goods_init_shipping message structure: <!-- (seller_id, quantity, price) -->
 
 
* \authors: Marko Petrovic
* \history: 09.11.2017-Marko: First implementation.
*/
int FGM_initialization()
{
	int seller_id = 0;
	double quantity = 0.0;
	double price = 0.0;
	
	reset_Final_good_array(&FINAL_GOODS_LIST);
	
	START_FINAL_GOODS_INIT_SHIPPING_MESSAGE_LOOP
	
		seller_id = final_goods_init_shipping_message->seller_id;
		quantity = final_goods_init_shipping_message->quantity;
		price = final_goods_init_shipping_message->price;
		
		add_Final_good(&FINAL_GOODS_LIST, seller_id, quantity, price, 0);
	
    FINISH_FINAL_GOODS_INIT_SHIPPING_MESSAGE_LOOP
	
	INITIALIZATION = 0;

    return 0;
}