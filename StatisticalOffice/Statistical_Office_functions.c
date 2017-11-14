#include "../header.h"
#include "../my_library_header.h"

#include "../Statistical_Office_agent_header.h"

int SO_idle()
{

    return 0;
}


/* \fn: int Statistical_Office_send_info()
 
* \brief: Statistical_Office send info.
 
 
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Statistical_Office_send_info()
{


    return 0;
}


/* \fn: int Statistical_Office_receive_data()
 
* \brief: Statistical Office receive data.
 
*\ fgm_send_statistics_message structure: <!-- (sold_quantity, price_level, market_turnover) -->

*\ hh_send_statistics_message structure: <!-- (hh_id, employer_id, wage) -->

*\ firm_send_statistics_message structure: <!-- (firm_id, status, physical_capital_stock, inventories, production) -->

*\ bank_send_statistics_message structure: <!-- (bank_id, value_at_risk) -->

*\ pcfirm_send_statistics_message structure: <!-- (pcfirm_id, monthly_investments, physical_capital_price, real_monthly_investments) -->
 
 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int Statistical_Office_receive_data()
{
	//Final goods market
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		double price_level;
		
		START_FGM_SEND_STATISTICS_MESSAGE_LOOP
		
			AGGREGATE_MONTHLY_SOLD_QUANTITY = fgm_send_statistics_message->sold_quantity;
			price_level = fgm_send_statistics_message->price_level;
			AGGREGATE_MONTHLY_TURNOVER = fgm_send_statistics_message->market_turnover;
			
		FINISH_FGM_SEND_STATISTICS_MESSAGE_LOOP
		
		if(price_level > 0)
		{
			remove_double(&PRICE_LEVEL_LIST,0);
			add_double(&PRICE_LEVEL_LIST,price_level);
		}
		
		double average_price_level = 0.0;
		
		int i;
		for(i = 0; i < PRICE_LEVEL_LIST.size; i++)
		{
			average_price_level += PRICE_LEVEL_LIST.array[i];
		}
		
		if(PRICE_LEVEL_LIST.size > 0) average_price_level = average_price_level/PRICE_LEVEL_LIST.size;
		
		if(average_price_level > 0)
		{
			remove_double(&AVERAGE_PRICE_LEVEL_LIST,0);
			add_double(&AVERAGE_PRICE_LEVEL_LIST, average_price_level);
			PRICE_LEVEL = average_price_level;
		}
		
		if(AVERAGE_PRICE_LEVEL_LIST.array[0] > 0)
		{
			PRICE_GROWTH = (AVERAGE_PRICE_LEVEL_LIST.array[11] - AVERAGE_PRICE_LEVEL_LIST.array[0])/AVERAGE_PRICE_LEVEL_LIST.array[0]; // THIS IS YEARLY INFLATION
		}
		else
		{
			PRICE_GROWTH = 0;
		}
		
		if (DAY < 240)
		PRICE_GROWTH = 0.02;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Households
	// hh_send_statistics_message structure: <!-- (hh_id, employer_id, wage) -->
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		int employer_id;
		int num_of_employees = 0;
		int num_of_unemployers = 0;
		int num_of_hh = 0;
		double wage;
		
		AVERAGE_WAGE = 0.0;
		UNEMPLOYMENT_RATE = 0.0;
		
		START_HH_SEND_STATISTICS_MESSAGE_LOOP
		
			employer_id = hh_send_statistics_message->employer_id;
			wage = hh_send_statistics_message->wage;
			num_of_hh++;
			
			if(employer_id == -1)
			{
				num_of_unemployers++;
			}
			else
			{
				num_of_employees++;
				AVERAGE_WAGE += wage;
			}		
			
		FINISH_HH_SEND_STATISTICS_MESSAGE_LOOP
		
		if(num_of_employees > 0)
		AVERAGE_WAGE = AVERAGE_WAGE/num_of_employees;
	
		if(num_of_hh > 0)
		UNEMPLOYMENT_RATE = (num_of_unemployers/num_of_hh);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Firm
	// firm_send_statistics_message structure: <!-- (firm_id, status, physical_capital_stock, inventories, production) -->
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		int status;
		int num_of_firms = 0;
		int num_of_bankruptcies = 0;
		double agregate_capital_stock = 0.0;
		double agregate_inventory_stock = 0.0;
		double agregate_production = 0.0;
	
		START_FIRM_SEND_STATISTICS_MESSAGE_LOOP
		
			num_of_firms++;
			status = firm_send_statistics_message->status;
			
			if(status == 0)
			{
				num_of_bankruptcies++;
			}
			else
			{
				agregate_capital_stock += firm_send_statistics_message->physical_capital_stock;
				agregate_inventory_stock += firm_send_statistics_message->inventories;
				agregate_production += firm_send_statistics_message->production;
			}

		FINISH_FIRM_SEND_STATISTICS_MESSAGE_LOOP
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Bank
	// bank_send_statistics_message structure: <!-- (bank_id, value_at_risk) -->
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		START_BANK_SEND_STATISTICS_MESSAGE_LOOP

			AGGREGATE_VALUE_AT_RISK = bank_send_statistics_message->value_at_risk;

		FINISH_BANK_SEND_STATISTICS_MESSAGE_LOOP
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	// PCFirm
	// pcfirm_send_statistics_message structure: <!-- (pcfirm_id, monthly_investments, physical_capital_price, real_monthly_investments) -->
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		double monthly_investments = 0.0;
		double physical_capital_price = 0.0;
		double real_monthly_investments = 0.0;
		
	
		START_PCFIRM_SEND_STATISTICS_MESSAGE_LOOP
		
			monthly_investments = pcfirm_send_statistics_message->monthly_investments;
			physical_capital_price = pcfirm_send_statistics_message->physical_capital_price;
			real_monthly_investments = pcfirm_send_statistics_message->real_monthly_investments;

		FINISH_PCFIRM_SEND_STATISTICS_MESSAGE_LOOP
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	NOMINAL_MONTHLY_GDP = AGGREGATE_MONTHLY_TURNOVER;
	REAL_MONTHLY_GDP = AGGREGATE_MONTHLY_SOLD_QUANTITY;

	
	FILE *file1;
	char *filename;

	filename = malloc(120*sizeof(char));
	filename[0]=0;
	strcpy(filename, "its/Statistical_Office_end_of_the_month_statistics.txt");      
	file1 = fopen(filename,"a");
	fprintf(file1,"\n %d %d %f %f %f %f",DAY, ID, NOMINAL_MONTHLY_GDP, REAL_MONTHLY_GDP, PRICE_LEVEL, PRICE_GROWTH);
	fprintf(file1," %f %f %d",AVERAGE_WAGE, UNEMPLOYMENT_RATE, num_of_bankruptcies);
	fprintf(file1," %f %f %f",agregate_capital_stock, agregate_inventory_stock, agregate_production);
	fprintf(file1," %f %f %f %f",AGGREGATE_VALUE_AT_RISK, monthly_investments, physical_capital_price, real_monthly_investments);
	fclose(file1);
	free(filename);

    return 0;
}

/* \fn: int Statistical_Office_send_data()
 
* \brief: Statistical Office send data to the government and the CB.
 
 stat_office_send_statistics_message structure: <!-- (monthly_gdp, price_level, inflation, value_at_risk, mean_wage, unemployment_rate) -->

* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int Statistical_Office_send_data()
{
	add_stat_office_send_statistics_message(NOMINAL_MONTHLY_GDP, PRICE_LEVEL, 
	PRICE_GROWTH, AGGREGATE_VALUE_AT_RISK, AVERAGE_WAGE, UNEMPLOYMENT_RATE);

    return 0;
}