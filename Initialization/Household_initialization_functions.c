#include "../header.h"
#include "../my_library_header.h"

#include "../Household_agent_header.h"



/* \fn: int Household_initialization()
 
* \brief: Household send data to government.
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
   *\ init_hh_to_gov message structure: <!-- (hh_id) -->
 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_initialization()
{
	add_init_hh_to_gov_message(ID);

    return 0;
}

/* \fn: int Household_receive_initialization()
 
* \brief: Household receive initialization and set initialization to zero.
 
*\ gov_to_hh_init_shares message structure: <!-- (hh_id, enterprise_id, shares, share_book_value) -->
 
 *\ filter: a.id==m.hh_id
 
 *\ Asset data type: <!-- (asset_id, number_of_assets, book_value) -->
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 

 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Household_receive_initialization()
{
	reset_Asset_array(&ASSETS_LIST);
	
	START_GOV_TO_HH_INIT_SHARES_MESSAGE_LOOP
	
		int present = 0;
		
		for(int i = 0; i < ASSETS_LIST.size; i++)
		{
			if(ASSETS_LIST.array[i].asset_id == gov_to_hh_init_shares_message->enterprise_id)
			{
				ASSETS_LIST.array[i].number_of_assets += gov_to_hh_init_shares_message->shares;
				present = 1;
				break;
			}
		}
		
		if(present == 0)
		{
			add_Asset(&ASSETS_LIST,
			gov_to_hh_init_shares_message->enterprise_id,
			gov_to_hh_init_shares_message->shares,gov_to_hh_init_shares_message->share_book_value);
		}			
	
    FINISH_GOV_TO_HH_INIT_SHARES_MESSAGE_LOOP
	
	
	
	INITIALIZATION = 0;
    return 0;
}