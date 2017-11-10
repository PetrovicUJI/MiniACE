#include "../header.h"
#include "../my_library_header.h"

#include "../PCFirm_agent_header.h"



/* \fn: int PCFirm_receive_physical_capital_order()
 
* \brief: Physical capital producer receive physical capital orders.
 
 
* \timing: Daily.
 * \condition: .
 
 physical_capital_order_message structure <!-- (buyer_id, quantity) -->
 
 physical_capital_delivery message structure <!-- (seller_id, buyer_id, quantity, price, technology) -->
 
 
* \authors: Marko Petrovic
* \history: 23.10.2017-Marko: First implementation.
*/
int PCFirm_receive_physical_capital_orders()
{
	int buyer_id = 0;
	double quantity = 0.0;
	
	
	START_PHYSICAL_CAPITAL_ORDER_MESSAGE_LOOP
	
		buyer_id = physical_capital_order_message->buyer_id;
		quantity = physical_capital_order_message->quantity;
		
		add_physical_capital_delivery_message(ID, buyer_id, quantity, PRICE, TECHNOLOGY);
		
		PAYMENT_ACCOUNT += quantity*PRICE;
		
		MONTHLY_REVENUE += quantity*PRICE;
		
    FINISH_PHYSICAL_CAPITAL_ORDER_MESSAGE_LOOP
	

    return 0;
}

/* \fn: int PCFirm_pay_dividends()
 
* \brief: PCFirm pay dividends.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 
*\ dividend_payment message structure: <!-- (firm_id, dividend) -->


* \authors: Marko Petrovic
* \history: 27.10.2017-Marko: First implementation.
*/
int PCFirm_pay_dividends()
{
	if(MONTHLY_REVENUE == 0)
	return 0;
	
	if(OUTSTANDING_SHARES == 0)
	printf("\n ERROR in function PCFirm_pay_dividends: OUTSTANDING_SHARES = %2.5f\n ", OUTSTANDING_SHARES);
	
	double dividend = 0.0;

	dividend = (MONTHLY_REVENUE/OUTSTANDING_SHARES);
	
	add_dividend_payment_message(ID, dividend);
	
	
	PAYMENT_ACCOUNT -= MONTHLY_REVENUE;
	


    return 0;
}


