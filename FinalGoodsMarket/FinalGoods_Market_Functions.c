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
		
		int present = 0;
		for(int i = 0; i < FINAL_GOODS_LIST.size; i++)
		{
			if (FINAL_GOODS_LIST.array[i].seller_id == seller_id)
			{
				present = 1;
				
				// remove a seller from the market!
				if(FINAL_GOODS_LIST.array[i].price == -1)
				{
					remove_Final_good(&FINAL_GOODS_LIST,i);
					break;
				}
				else
				{
					FINAL_GOODS_LIST.array[i].quantity += quantity;
					FINAL_GOODS_LIST.array[i].price = price;
					break;
				}
			}
		}
		
		if(present == 0 && final_goods_shipping_message->price > 0)
		add_Final_good(&FINAL_GOODS_LIST, seller_id, quantity, price, 0);
	
    FINISH_FINAL_GOODS_SHIPPING_MESSAGE_LOOP
	
	for(int i = 0; i < FINAL_GOODS_LIST.size; i++)
	{
		seller_id = FINAL_GOODS_LIST.array[i].seller_id;
		quantity = FINAL_GOODS_LIST.array[i].quantity;
		price = FINAL_GOODS_LIST.array[i].price;
		
		if(DEBUG_MODE)
		{
			FILE *file1;
			char *filename;

			filename = malloc(120*sizeof(char));
			filename[0]=0;
			strcpy(filename, "its/FGM_supply.txt");      
			file1 = fopen(filename,"a");
			fprintf(file1,"\n %d %d %d %f %f",DAY, ID, seller_id, quantity, price);
			fclose(file1);
			free(filename);
		}
	}
		
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
	
	for(int i = 0; i < FINAL_GOODS_ORDERS_LIST.size; i++)
	{
		buyer_id = FINAL_GOODS_ORDERS_LIST.array[i].buyer_id;
		budget = FINAL_GOODS_ORDERS_LIST.array[i].budget;
		
		
		if(DEBUG_MODE)
		{
			FILE *file1;
			char *filename;

			filename = malloc(120*sizeof(char));
			filename[0]=0;
			strcpy(filename, "its/FGM_demand.txt");      
			file1 = fopen(filename,"a");
			fprintf(file1,"\n %d %d %d %f",DAY, ID, buyer_id, budget);
			fclose(file1);
			free(filename);
		}
	}

	
    return 0;
}


/* \fn: int Final_goods_market_send_products_to_buyers()
 
* \brief: Final goods market send products.
 
 *\ data type Final_good structure: <!-- (seller_id, quantity, price, sold_quantity) -->
 
 *\ final_goods_order_message structure	  		<!-- (buyer_id, budget) -->
  
 *\ final_goods_delivery_to_buyers message structure: <!-- (buyer_id, remaining_budget, costs, purchased_quantity) -->
 
* \timing: Daily
* \timing: 
* \condition:
 
 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Final_goods_market_send_products_to_buyers()
{
	if(FINAL_GOODS_ORDERS_LIST.size == 0 || FINAL_GOODS_LIST.size == 0)
	{
		printf("\n WARNING in function Final_goods_market_send_products_to_buyers: FINAL_GOODS_ORDERS_LIST.size = %2.5f\n ", FINAL_GOODS_ORDERS_LIST.size);
		printf("\n WARNING in function Final_goods_market_send_products_to_buyers: FINAL_GOODS_LIST.size = %2.5f\n ", FINAL_GOODS_LIST.size);
		return 0;
	}

	// set daily sold quantities to zero and make sure that there is no negative or zero price
	int j;
	for(j = 0; j < FINAL_GOODS_LIST.size; j++)
	{
		FINAL_GOODS_LIST.array[j].sold_quantity = 0;
		
		if(FINAL_GOODS_LIST.array[j].price <= 0)
		printf("\n ERROR in function Final_goods_market_send_products_to_buyers: PRICE = %2.5f\n ", FINAL_GOODS_LIST.array[j].price);
	}
	
	
	int index = 0;
	
	// buyers observe only 20% of sellers on the market
	int max_sellers = (int)ceil(FINAL_GOODS_LIST.size*0.2);
	
	Final_good_array  final_goods_list;
    init_Final_good_array(&final_goods_list);
	
	int buyer_id;
	double costs;
	double purchased_quantity;
	
	for(int i = 0; i < FINAL_GOODS_ORDERS_LIST.size; i++)
	{
		buyer_id = FINAL_GOODS_ORDERS_LIST.array[i].buyer_id;
		
		costs = 0.0;
		purchased_quantity = 0.0;
		
		j = 0;
		while(j < max_sellers && FINAL_GOODS_LIST.size > 0)
		{
			index = random_int(0, (FINAL_GOODS_LIST.size-1));
		
			add_Final_good(&final_goods_list,
			FINAL_GOODS_LIST.array[index].seller_id,
			FINAL_GOODS_LIST.array[index].quantity,
			FINAL_GOODS_LIST.array[index].price,
			FINAL_GOODS_LIST.array[index].sold_quantity);
		
			// the seller is temporary taken out, but later is added back
			remove_Final_good(&FINAL_GOODS_LIST, index);
			j++;
		}
		
		int seller_id = 0;
		double quantity = 0.0;
		double price = 0.0;
		double sold = 0.0;
		
		for(j = 0; j < final_goods_list.size; j++)
		{
			
			seller_id = final_goods_list.array[j].seller_id,
			quantity = final_goods_list.array[j].quantity,
			price = final_goods_list.array[j].price,
			sold = final_goods_list.array[j].sold_quantity;
			
			if(DEBUG_MODE)
			{
				FILE *file1;
				char *filename;

				filename = malloc(120*sizeof(char));
				filename[0]=0;
				strcpy(filename, "its/FGM_final_goods_list_order_1.txt");      
				file1 = fopen(filename,"a");
				fprintf(file1,"\n %d %d %d %f %f %f",DAY, buyer_id, seller_id, quantity, price, sold);
				fclose(file1);
				free(filename);
			}
		}

		/*Ranks goods regarding their price, from the highest to the lower - thus the last price in the array will be the lowest.*/
		qsort(final_goods_list.array, final_goods_list.size, sizeof(Final_good),final_goods_list_rank_price_function);
		
		for(j = 0; j < final_goods_list.size; j++)
		{
			
			seller_id = final_goods_list.array[j].seller_id,
			quantity = final_goods_list.array[j].quantity,
			price = final_goods_list.array[j].price,
			sold = final_goods_list.array[j].sold_quantity;
			
			if(DEBUG_MODE)
			{
				FILE *file1;
				char *filename;

				filename = malloc(120*sizeof(char));
				filename[0]=0;
				strcpy(filename, "its/FGM_final_goods_list_order_2.txt");      
				file1 = fopen(filename,"a");
				fprintf(file1,"\n %d %d %d %f %f %f",DAY, buyer_id, seller_id, quantity, price, sold);
				fclose(file1);
				free(filename);
			}
		}

		j = final_goods_list.size-1;
		while(j >= 0 && FINAL_GOODS_ORDERS_LIST.array[i].budget > 0)
		{
			if(final_goods_list.array[j].quantity*final_goods_list.array[j].price >= FINAL_GOODS_ORDERS_LIST.array[i].budget)
			{
				purchased_quantity += FINAL_GOODS_ORDERS_LIST.array[i].budget/final_goods_list.array[j].price;
				costs += FINAL_GOODS_ORDERS_LIST.array[i].budget;
				
				final_goods_list.array[j].sold_quantity += FINAL_GOODS_ORDERS_LIST.array[i].budget/final_goods_list.array[j].price;
				final_goods_list.array[j].quantity -= FINAL_GOODS_ORDERS_LIST.array[i].budget/final_goods_list.array[j].price;
				
				FINAL_GOODS_ORDERS_LIST.array[i].budget = 0;
			}
			else
			{
				purchased_quantity += final_goods_list.array[j].quantity;
				costs += final_goods_list.array[j].quantity*final_goods_list.array[j].price;
				FINAL_GOODS_ORDERS_LIST.array[i].budget -= final_goods_list.array[j].quantity*final_goods_list.array[j].price;
				
				final_goods_list.array[j].sold_quantity += final_goods_list.array[j].quantity;
				final_goods_list.array[j].quantity = 0;
			}
			j--;
		}
		add_final_goods_delivery_to_buyers_message(buyer_id, FINAL_GOODS_ORDERS_LIST.array[i].budget, costs, purchased_quantity);
		
		
		if(DEBUG_MODE)
		{
			FILE *file1;
			char *filename;

			filename = malloc(120*sizeof(char));
			filename[0]=0;
			strcpy(filename, "its/FGM_final_goods_delivery.txt");      
			file1 = fopen(filename,"a");
			fprintf(file1,"\n %d %d %f %f %f",DAY, buyer_id, FINAL_GOODS_ORDERS_LIST.array[i].budget, costs, purchased_quantity);
			fclose(file1);
			free(filename);
		}

		

		for(j = 0; j < final_goods_list.size; j++)
		{
			add_Final_good(&FINAL_GOODS_LIST,
			final_goods_list.array[j].seller_id,
			final_goods_list.array[j].quantity,
			final_goods_list.array[j].price,
			final_goods_list.array[j].sold_quantity);
			
			seller_id = final_goods_list.array[j].seller_id,
			quantity = final_goods_list.array[j].quantity,
			price = final_goods_list.array[j].price,
			sold = final_goods_list.array[j].sold_quantity;
			
			if(DEBUG_MODE)
			{
				FILE *file1;
				char *filename;

				filename = malloc(120*sizeof(char));
				filename[0]=0;
				strcpy(filename, "its/FGM_final_goods_turnover.txt");      
				file1 = fopen(filename,"a");
				fprintf(file1,"\n %d %d %f %f %f",DAY, seller_id, quantity, price, sold);
				fclose(file1);
				free(filename);
			}
		}
		
		reset_Final_good_array(&final_goods_list);
	}
	
	free_Final_good_array(&final_goods_list);

    return 0;
}

/* \fn: int Final_goods_market_update_sellers()
 
* \brief: Final goods market update sellers.
 
 
 *\ final_goods_market_transaction_update message structure: <!-- (seller_id, sold_quantity, revenue) -->
 
 *\ data type Final_good structure: <!-- (seller_id, quantity, price, sold_quantity) -->
 
 filter: a.id == m.seller_id
 
* \timing: Daily.
 * \condition:
 

* \authors: Marko Petrovic
* \history: 26.10.2017-Marko: First implementation.
*/
int Final_goods_market_update_sellers()
{
	DAILY_SOLD_QUANTITY = 0.0;
	DAILY_MARKET_TURNOVER = 0.0;
	
	for(int i = 0; i < FINAL_GOODS_LIST.size; i++)
	{
		add_final_goods_market_transaction_update_message(
			FINAL_GOODS_LIST.array[i].seller_id,
			FINAL_GOODS_LIST.array[i].sold_quantity,
			FINAL_GOODS_LIST.array[i].sold_quantity*FINAL_GOODS_LIST.array[i].price
		);
		
		DAILY_SOLD_QUANTITY += FINAL_GOODS_LIST.array[i].sold_quantity;
		DAILY_MARKET_TURNOVER += FINAL_GOODS_LIST.array[i].sold_quantity*FINAL_GOODS_LIST.array[i].price;
		
		
		if(DEBUG_MODE)
		{
			FILE *file1;
			char *filename;

			filename = malloc(120*sizeof(char));
			filename[0]=0;
			strcpy(filename, "its/DAILY_SOLD_QUANTITY.txt");      
			file1 = fopen(filename,"a");
			fprintf(file1,"\n %f %f",DAILY_SOLD_QUANTITY, DAILY_MARKET_TURNOVER);
			fclose(file1);
			free(filename);
		}


		FINAL_GOODS_LIST.array[i].sold_quantity = 0.0;
	}
	
	MONTHLY_SOLD_QUANTITY += DAILY_SOLD_QUANTITY;
	MONTHLY_MARKET_TURNOVER += DAILY_MARKET_TURNOVER;
	
    return 0;
}