#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"



/* \fn: int Firm_send_physical_capital_order()
 
* \brief: Firm send physical capital order.
 
 
* \timing: Activation day.
 * \condition: If investment plan > 0.
 
 physical_capital_order_message structure <!-- (buyer_id, quantity) -->
  
* \authors: Marko Petrovic
* \history: 23.10.2017-Marko: First implementation.
*/
int Firm_send_physical_capital_order()
{
	add_physical_capital_order_message(ID, INVESTMENT_PLAN);
	
    return 0;
}


/* \fn: int Firm_receive_physical_capital()
 
* \brief: Firm receive physical capital.
 
 
* \timing: Activation day.
 * \condition: If investment plan > 0.
 
 *\ physical_capital_delivery message structure <!-- (seller_id, buyer_id, quantity, price, technology) -->
 
 *\ Physical_capital_batch data structure:
 <!-- id, months_in_use, capital_units, price, current_value, productivity  -->
 
 *\ message filter: if a.id == m.buyer_id
 
* \authors: Marko Petrovic
* \history: 23.10.2017-Marko: First implementation.
*/
int Firm_receive_physical_capital()
{
	
	START_PHYSICAL_CAPITAL_DELIVERY_MESSAGE_LOOP
	
		int seller_id = 0;
		double quantity = 0.0;
		double price = 0.0;
		double technology = 0.0;
	
		seller_id = physical_capital_delivery_message->seller_id;
		quantity = physical_capital_delivery_message->quantity;
		price = physical_capital_delivery_message->price;
		technology = physical_capital_delivery_message->technology;
		
		add_Physical_capital_batch(&PHYSICAL_CAPITAL_STOCK, 
		seller_id, 0, quantity, price, price*quantity, technology);
		
		PAYMENT_ACCOUNT -= quantity*price;
		
		
		PHYSICAL_CAPITAL += quantity;		
		NON_CURRENT_ASSETS += price*quantity;
		
		
    FINISH_PHYSICAL_CAPITAL_DELIVERY_MESSAGE_LOOP
	

    return 0;
}

