#include "../header.h"
#include "../my_library_header.h"

#include "../Government_agent_header.h"



/* \fn: int Government_initialization()
 
* \brief: Government set initialization to zero.

*\ init_hh_to_gov message structure: <!-- (hh_id) -->
 
*\ init_firm_to_gov message structure: <!-- (firm_id, outstanding_shares) -->
 
*\ init_bank_to_gov message structure: <!-- (bank_id, outstanding_shares) -->

*\ init_pcfirm_to_gov message structure: <!-- (pcfirm_id, outstanding_shares) -->


*\ 	gov_to_hh_init_shares message structure <!-- (hh_id, enterprise_id, shares, share_book_value) -->
 
 
 *\ Enterprise_init data type structure <!-- (enterprise_id, outstanding_shares, share_book_value) -->
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Government_initialization()
{	
	Enterprise_init_array firm_list;
	init_Enterprise_init_array(&firm_list);
	
	Enterprise_init_array pcfirm_list;
	init_Enterprise_init_array(&pcfirm_list);
	
	Enterprise_init_array bank_list;
	init_Enterprise_init_array(&bank_list);
	
	int_array hh_list;
	init_int_array(&hh_list);
	
	START_INIT_HH_TO_GOV_MESSAGE_LOOP
	
		add_int(&hh_list, init_hh_to_gov_message->hh_id);
	
    FINISH_INIT_HH_TO_GOV_MESSAGE_LOOP
	
	// initialize firm balance sheet
	
	double total_production = hh_list.size; // given prices and wages equal to 1.
	double firm_production = total_production/firm_list.size;
	double firm_capital = firm_production; // given productivity = 1
	double firm_inventories = 0.2*firm_production;
	
	double firm_equity = firm_capital+firm_inventories;
	
	
	// firm payment_account, non_current_liabilities and current_liabilities are equal to ZERO!
	
	// initialize bank balance sheet
	
	double bank_deposits = hh_list.size; // each household is endowed with 1 unit of money.
	double bank_equity = bank_deposits/2;
	//double bank_payment_account = bank_deposits + bank_equity;
	
	START_INIT_FIRM_TO_GOV_MESSAGE_LOOP
		
		add_Enterprise_init(&firm_list,
		init_firm_to_gov_message->firm_id,
		init_firm_to_gov_message->outstanding_shares,
		firm_equity/init_firm_to_gov_message->outstanding_shares);
	
    FINISH_INIT_FIRM_TO_GOV_MESSAGE_LOOP
	
	START_INIT_PCFIRM_TO_GOV_MESSAGE_LOOP
		
		add_Enterprise_init(&pcfirm_list,
		init_pcfirm_to_gov_message->pcfirm_id,
		init_pcfirm_to_gov_message->outstanding_shares, 0);
		
	
    FINISH_INIT_PCFIRM_TO_GOV_MESSAGE_LOOP
	
	START_INIT_BANK_TO_GOV_MESSAGE_LOOP
		
		add_Enterprise_init(&bank_list,
		init_bank_to_gov_message->bank_id,
		init_bank_to_gov_message->outstanding_shares,
		bank_equity/init_bank_to_gov_message->outstanding_shares);
	
    FINISH_INIT_BANK_TO_GOV_MESSAGE_LOOP
	

	int i = 0;
	int j = 0;
	while(firm_list.size > 0)
	{
		if(i == firm_list.size)
		i = 0;
	
		if(j == hh_list.size)
		j = 0;
	
		add_gov_to_hh_init_shares_message(hh_list.array[j], firm_list.array[i].enterprise_id, 
		1, firm_list.array[i].share_book_value);
		
		firm_list.array[i].outstanding_shares--;
		
		if(firm_list.array[i].outstanding_shares == 0)
		remove_Enterprise_init(&firm_list, i);	

		i++;
		j++;
	}
	
	i = 0;
	while(pcfirm_list.size > 0)
	{
		if(i == pcfirm_list.size)
		i = 0;
	
		if(j == hh_list.size)
		j = 0;
	
		add_gov_to_hh_init_shares_message(hh_list.array[j], pcfirm_list.array[i].enterprise_id, 
		1, pcfirm_list.array[i].share_book_value);
		
		pcfirm_list.array[i].outstanding_shares--;
		
		if(pcfirm_list.array[i].outstanding_shares == 0)
		remove_Enterprise_init(&pcfirm_list, i);	

		i++;
		j++;
	}
	
	i = 0;
	while(bank_list.size > 0)
	{
		if(i == bank_list.size)
		i = 0;
	
		if(j == hh_list.size)
		j = 0;
	
		add_gov_to_hh_init_shares_message(hh_list.array[j], bank_list.array[i].enterprise_id, 
		1, bank_list.array[i].share_book_value);
		
		bank_list.array[i].outstanding_shares--;
		
		if(bank_list.array[i].outstanding_shares == 0)
		remove_Enterprise_init(&bank_list, i);	

		i++;
		j++;
	}
	
	
	
	
	free_int_array(&hh_list);
	free_Enterprise_init_array(&firm_list);
	free_Enterprise_init_array(&pcfirm_list);
	free_Enterprise_init_array(&bank_list);
	
	INITIALIZATION = 0;

    return 0;
}