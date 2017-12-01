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
	if(DEBUG_MODE)
	{
		FILE *file1;
		char *filename;
		
		filename = malloc(120*sizeof(char));
		filename[0]=0;
		strcpy(filename, "its/Firm_stat.txt");      
		file1 = fopen(filename,"a");
		fprintf(file1,"\n %d %d %f %f %f %f %f",DAY, ID, TOTAL_ASSETS, CURRENT_ASSETS, INVENTORIES, NON_CURRENT_ASSETS, PAYMENT_ACCOUNT);
		fprintf(file1," %f %f %f %f",TOTAL_LIABILITIES, CURRENT_LIABILITIES, NON_CURRENT_LIABILITIES, EQUITY);
		fclose(file1);
		free(filename);
	}

	
	START_FINAL_GOODS_MARKET_TRANSACTION_UPDATE_MESSAGE_LOOP
	
		MONTHLY_SOLD_QUANTITY += final_goods_market_transaction_update_message->sold_quantity;
		INVENTORIES -= final_goods_market_transaction_update_message->sold_quantity;
		PAYMENT_ACCOUNT += final_goods_market_transaction_update_message->revenue;
		MONTHLY_REVENUE += final_goods_market_transaction_update_message->revenue;
		
    FINISH_FINAL_GOODS_MARKET_TRANSACTION_UPDATE_MESSAGE_LOOP
	
	// inventories are estimated at average market price
	CURRENT_ASSETS = INVENTORIES*PRICE_LEVEL;
	
	if(DEBUG_MODE)
	{
		FILE *file1;
		char *filename;
		
		filename = malloc(120*sizeof(char));
		filename[0]=0;
		strcpy(filename, "its/Firm_stat.txt");      
		file1 = fopen(filename,"a");
		fprintf(file1,"\n %d %d %f %f %f %f %f",DAY, ID, TOTAL_ASSETS, CURRENT_ASSETS, INVENTORIES, NON_CURRENT_ASSETS, PAYMENT_ACCOUNT);
		fprintf(file1," %f %f %f %f",TOTAL_LIABILITIES, CURRENT_LIABILITIES, NON_CURRENT_LIABILITIES, EQUITY);
		fclose(file1);
		free(filename);
	}
	

    return 0;
}


