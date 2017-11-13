#include "../header.h"
#include "../my_library_header.h"

#include "../Household_agent_header.h"


/* \fn: int Household_receive_info()
 
* \brief: Household receive info.
 
  *\ enterprise_to_hh_info message structure: 	<!-- (enterprise_id, share_book_value) -->
  
  *\ Asset data type: <!-- (asset_id, number_of_assets, book_value) -->
  


 *\ gov_send_info_message structure: 
 <!-- (mean_wage, vat_rate, income_tax_rate, capital_tax_rate, 
		unemployment_benefits_rate, public_transfers_rate) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Household_receive_info()
{
	NON_CURRENT_ASSETS = 0.0;
	
	START_ENTERPRISE_TO_HH_INFO_MESSAGE_LOOP
		
		for(int i = 0; i < ASSETS_LIST.size; i++)
		{
			if(ASSETS_LIST.array[i].asset_id == enterprise_to_hh_info_message->enterprise_id)
			{
				ASSETS_LIST.array[i].book_value = enterprise_to_hh_info_message->share_book_value;
				
				NON_CURRENT_ASSETS += ASSETS_LIST.array[i].book_value*ASSETS_LIST.array[i].number_of_assets;
				break;
			}
		}			
	
    FINISH_ENTERPRISE_TO_HH_INFO_MESSAGE_LOOP
	
	
	if(DAY%20 == 1)
	{
		START_GOV_SEND_INFO_MESSAGE_LOOP
	
			PUBLIC_TRANSFERS_RATE = gov_send_info_message->public_transfers_rate;
			UNEMPLOYMENT_BENEFITS_RATE = gov_send_info_message->unemployment_benefits_rate;
			MEAN_WAGE = gov_send_info_message->mean_wage;
			
			CAPITAL_TAX_RATE = gov_send_info_message->capital_tax_rate;
			INCOME_TAX_RATE = gov_send_info_message->income_tax_rate;
	
		FINISH_GOV_SEND_INFO_MESSAGE_LOOP
		
		PUBLIC_TRANSFERS = PUBLIC_TRANSFERS_RATE*MEAN_WAGE;
		UNEMPLOYMENT_BENEFIT = WAGE*UNEMPLOYMENT_BENEFITS_RATE;
		
		MIN_RESERVATION_WAGE = 0.7*MEAN_WAGE;
	}
	

    return 0;
}

/* \fn: int Household_send_statistics()
 
* \brief: Household send statistics.
 
 
* \timing: Monthly, the last day of the month.

 * \condition:

 
 *\ hh_send_statistics_message structure: <!-- (hh_id, employer_id, wage) -->
 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int Household_send_statistics()
{

	add_hh_send_statistics_message(ID, EMPLOYER_ID, INCOME);
	

	return 0;
}
