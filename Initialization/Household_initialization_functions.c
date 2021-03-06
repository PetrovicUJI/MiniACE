#include "../header.h"
#include "../my_library_header.h"

#include "../Household_agent_header.h"



/* \fn: int Household_initialization()
 
* \brief: Household send data to government.
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
   *\ hh_to_gov_ini_message structure: <!-- (hh_id) -->
 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_initialization()
{
	add_hh_to_gov_ini_message(ID);

    return 0;
}

/* \fn: int Household_receive_initialization()
 
* \brief: Household receive initialization and set initialization to zero.
 
*\ gov_to_hh_init_shares message structure: <!-- (hh_id, enterprise_id, shares, share_book_value) -->
 
 *\ filter: a.id==m.hh_id
 
 
  *\ gov_init_labor_message structure: <!-- (hh_id, employer_id, activation_day, wage) -->
 filters: households: a.id == m.hh_id;    firms:  a.id == m.employer_id
 
*\gov_init_balance_sheets message structure:
*\ <!-- (agent_id, current_assets, non_current_assets, payment_account, current_liabilities, non_current_liabilities) -->
 filter: a.id == m.agent_id	
 
 
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
	
	START_GOV_INIT_LABOR_MESSAGE_LOOP
	
		EMPLOYER_ID = gov_init_labor_message->employer_id;
		DAY_OF_MONTH_TO_ACT = gov_init_labor_message->activation_day;
		WAGE = gov_init_labor_message->wage;
	
    FINISH_GOV_INIT_LABOR_MESSAGE_LOOP
	
	DAY_OF_MONTH_TO_RECEIVE_INCOME = (DAY_OF_MONTH_TO_ACT+19)%MONTH;
	
	DAY_OF_WEEK_TO_ACT = random_int(0,4);
	
	RESERVATION_WAGE = WAGE;
	
	
	START_GOV_INIT_BALANCE_SHEETS_MESSAGE_LOOP
	
		PAYMENT_ACCOUNT = gov_init_balance_sheets_message->payment_account;
	
    FINISH_GOV_INIT_BALANCE_SHEETS_MESSAGE_LOOP
	
	NON_CURRENT_ASSETS = 0;
	CURRENT_LIABILITIES = 0;
	NON_CURRENT_LIABILITIES = 0;
	
	CURRENT_ASSETS = 0;
	double number_of_assets = 0.0;
	for(int i = 0; i < ASSETS_LIST.size; i++)
	{
		CURRENT_ASSETS += ASSETS_LIST.array[i].number_of_assets*ASSETS_LIST.array[i].book_value;
		number_of_assets += ASSETS_LIST.array[i].number_of_assets;
	}
	
	TOTAL_ASSETS = CURRENT_ASSETS + NON_CURRENT_ASSETS + PAYMENT_ACCOUNT;
	TOTAL_LIABILITIES = CURRENT_LIABILITIES + NON_CURRENT_LIABILITIES;
	WEALTH = TOTAL_ASSETS - TOTAL_LIABILITIES;
	
	FILE *file1;
	char *filename;

	filename = malloc(120*sizeof(char));
	filename[0]=0;
	strcpy(filename, "its/Household_receive_initialization.txt");      
	file1 = fopen(filename,"a");
	fprintf(file1,"\n %d %d %f %f %f %f",DAY, ID, TOTAL_ASSETS, CURRENT_ASSETS, NON_CURRENT_ASSETS, PAYMENT_ACCOUNT);
	fprintf(file1," %f %f %f %f %f",TOTAL_LIABILITIES, CURRENT_LIABILITIES, NON_CURRENT_LIABILITIES, WEALTH, number_of_assets);
	fclose(file1);
	free(filename);
	
	
	
	// initialize consumption budget
	
	
	MONTHLY_CONSUMPTION_BUDGET = PAYMENT_ACCOUNT; 
	
	WEEK_COUNTER = 1;

	INITIALIZATION = 0;
    
	return 0;
}