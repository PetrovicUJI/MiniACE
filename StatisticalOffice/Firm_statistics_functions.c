#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"


/* \fn: int Firm_send_info()
 
* \brief: Firm send info to Firms.
 
 *\ enterprise_to_hh_info message structure: 	<!-- (enterprise_id, share_book_value) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Firm_send_info()
{
	
	double share_book_value = 0.0;
	
	if(ACTIVE == 1)
	{
		if(EQUITY < 0)
		printf("\n ERROR in function Firm_send_info: EQUITY = %2.5f\n ", EQUITY);
	
		if(OUTSTANDING_SHARES <= 0)
		printf("\n ERROR in function Firm_send_info: OUTSTANDING_SHARES = %2.5f\n ", OUTSTANDING_SHARES);
	
		share_book_value = EQUITY/OUTSTANDING_SHARES;
	}
	
	add_enterprise_to_hh_info_message(ID, share_book_value);


    return 0;
}



/* \fn: int Firm_receive_info()
 
* \brief: Firm receive info from Gov, CB and PCFIRMS.
 
 *\ cb_send_info_message structure: 
<!-- (price_level, inflation, cb_interest_rate, alpha_capital_requirement, inflation_target) -->


 *\ gov_send_info_message structure: 
 <!-- (mean_wage, vat_rate, income_tax_rate, capital_tax_rate, 
		unemployment_benefits_rate, public_transfers_rate) -->

		
 *\ pcfirm_send_info_message structure: <!-- (pcfirm_id, physical_capital_price) -->
 

* \authors: Marko Petrovic
* \history: 13.11.2017-Marko: First implementation.
*/
int Firm_receive_info()
{
	
	START_CB_SEND_INFO_MESSAGE_LOOP
		
		PRICE_LEVEL = cb_send_info_message->price_level;
		INFLATION_TARGET = cb_send_info_message->inflation_target;

	FINISH_CB_SEND_INFO_MESSAGE_LOOP
	
	START_GOV_SEND_INFO_MESSAGE_LOOP
	
			AVERAGE_WAGE_LEVEL = gov_send_info_message->mean_wage;
			
			CAPITAL_TAX_RATE = gov_send_info_message->capital_tax_rate;
			VAT_RATE = gov_send_info_message->vat_rate;
	
	FINISH_GOV_SEND_INFO_MESSAGE_LOOP
	
	
	START_PCFIRM_SEND_INFO_MESSAGE_LOOP
		
		PHYSICAL_CAPITAL_PRICE = pcfirm_send_info_message->physical_capital_price;

	FINISH_PCFIRM_SEND_INFO_MESSAGE_LOOP
	

    return 0;
}








/* \fn: int Firm_send_statistics()
 
* \brief: Firm send statistics.
 
 
* \timing: Monthly, the last day of the month.

 * \condition:

 
 *\ firm_send_statistics_message structure: <!-- (firm_id, status, physical_capital_stock, inventories, production) -->
 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int Firm_send_statistics()
{

	add_firm_send_statistics_message(ID, ACTIVE, PHYSICAL_CAPITAL, INVENTORIES, PRODUCTION);
	

	return 0;
}
