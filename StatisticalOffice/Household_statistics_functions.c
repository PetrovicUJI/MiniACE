#include "../header.h"
#include "../my_library_header.h"

#include "../Household_agent_header.h"


/* \fn: int Household_info()
 
* \brief: Household info.
 
  *\ enterprise_to_hh_info message structure: 	<!-- (enterprise_id, share_book_value) -->
  
  *\ Asset data type: <!-- (asset_id, number_of_assets, book_value) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Household_info()
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
