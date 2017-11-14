#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"

int Firm_idle()
{

    return 0;
}

/* \fn: int Firm_initialization()
 
* \brief: Firm send data to the government
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
 *\ init_firm_to_gov message structure: <!-- (firm_id, outstanding_shares) -->
  
* \authors: Marko Petrovic
* \history: 27.09.2017-Marko: First implementation.
*/
int Firm_initialization()
{
	add_init_firm_to_gov_message(ID, OUTSTANDING_SHARES);

    return 0;
}

/* \fn: int Firm_receive_initialization()
 
* \brief: Firm receive initialization. Firm set initialization to zero.
 
 
* \timing: First iteration only when initialization is needed.


  *\ gov_init_labor_message structure: <!-- (hh_id, employer_id, activation_day) -->
 filters: households: a.id == m.hh_id;    firms:  a.id == m.employer_id

*\gov_init_balance_sheets message structure:
*\ <!-- (agent_id, current_assets, non_current_assets, payment_account, current_liabilities, non_current_liabilities) -->
 filter: a.id == m.agent_id
 
 *\ pcfirm_price_info message structure: <!-- (pcfirm_id, pc_price, technology) -->
 
  *\ Physical_capital_batch data structure:
 <!-- id, months_in_use, capital_units, price, current_value, productivity  -->
 
 *\ final_goods_init_shipping message structure: <!-- (seller_id, quantity, price) -->
 
 
* \authors: Marko Petrovic
* \history: 09.11.2017-Marko: First implementation.
*/
int Firm_receive_initialization()
{
	PRICE = 1;
	
	reset_Employee_array(&EMPLOYEES);
	
	double wage = 1.0;
	
	START_GOV_INIT_LABOR_MESSAGE_LOOP
	
		add_Employee(&EMPLOYEES, gov_init_labor_message->hh_id, wage);
		
		DAY_OF_MONTH_TO_ACT = gov_init_labor_message->activation_day;
	
    FINISH_GOV_INIT_LABOR_MESSAGE_LOOP
	
	LAST_DAY_OF_MONTH_TO_ACT = (DAY_OF_MONTH_TO_ACT+19)%MONTH;


	int pcfirm_id = 0;
	START_PCFIRM_PRICE_INFO_MESSAGE_LOOP
	
		pcfirm_id = pcfirm_price_info_message->pcfirm_id;
		PHYSICAL_CAPITAL_PRICE = pcfirm_price_info_message->pc_price;
		PRODUCTIVITY = pcfirm_price_info_message->technology;
	
    FINISH_PCFIRM_PRICE_INFO_MESSAGE_LOOP
	
	
	START_GOV_INIT_BALANCE_SHEETS_MESSAGE_LOOP
	
		CURRENT_ASSETS = gov_init_balance_sheets_message->current_assets;
		PHYSICAL_CAPITAL = gov_init_balance_sheets_message->non_current_assets;
		PAYMENT_ACCOUNT = gov_init_balance_sheets_message->payment_account;
		CURRENT_LIABILITIES = gov_init_balance_sheets_message->current_liabilities;
		NON_CURRENT_LIABILITIES = gov_init_balance_sheets_message->non_current_liabilities;
	
    FINISH_GOV_INIT_BALANCE_SHEETS_MESSAGE_LOOP
	
	INVENTORIES = CURRENT_ASSETS;
	NON_CURRENT_ASSETS = PHYSICAL_CAPITAL*PHYSICAL_CAPITAL_PRICE;
	
	TOTAL_ASSETS = CURRENT_ASSETS + NON_CURRENT_ASSETS + PAYMENT_ACCOUNT;
	TOTAL_LIABILITIES = CURRENT_LIABILITIES + NON_CURRENT_LIABILITIES;
	EQUITY = TOTAL_ASSETS - TOTAL_LIABILITIES;
	
	reset_double_array(&SOLD_QUANTITIES_VECTOR);
	for(int i = 0; i <= 12; i++)
	{
		add_double(&SOLD_QUANTITIES_VECTOR,PHYSICAL_CAPITAL);
	}
	
	reset_Physical_capital_batch_array(&PHYSICAL_CAPITAL_STOCK);
	
	add_Physical_capital_batch(&PHYSICAL_CAPITAL_STOCK, 
	pcfirm_id, 0, PHYSICAL_CAPITAL, PHYSICAL_CAPITAL_PRICE, PHYSICAL_CAPITAL_PRICE*PHYSICAL_CAPITAL, PRODUCTIVITY);
	
	// initialize consumption goods market
	add_final_goods_init_shipping_message(ID, INVENTORIES, PRICE);
	

	INITIALIZATION = 0;

    return 0;
}