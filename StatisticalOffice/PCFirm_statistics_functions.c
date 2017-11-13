#include "../header.h"
#include "../my_library_header.h"

#include "../PCFirm_agent_header.h"


/* \fn: int PCFirm_send_info()
 
* \brief: PCFirm send info to hh.
 
 *\ enterprise_to_hh_info message structure: 	<!-- (enterprise_id, share_book_value) -->
 
 *\ pcfirm_send_info_message structure: <!-- (pcfirm_id, physical_capital_price) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int PCFirm_send_info()
{
	double share_book_value = 0.0;
	
	if(OUTSTANDING_SHARES <= 0)
	printf("\n ERROR in function Firm_send_info: OUTSTANDING_SHARES = %2.5f\n ", OUTSTANDING_SHARES);
	
	if(EQUITY > 0)
	share_book_value = EQUITY/OUTSTANDING_SHARES;

	add_enterprise_to_hh_info_message(ID, share_book_value);
	
	add_pcfirm_send_info_message(ID, PRICE);

    return 0;
}



/* \fn: int PCFirm_receive_info()
 
* \brief: PCFirm receive policy info.
 
 * \timing: Monthly, the first day of the month.
 
 
 *\ cb_send_info_message structure: 
<!-- (price_level, inflation, cb_interest_rate, alpha_capital_requirement, inflation_target) -->
 
* \authors: Marko Petrovic
* \history: 13.11.2017-Marko: First implementation.
*/
int PCFirm_receive_info()
{
	START_CB_SEND_INFO_MESSAGE_LOOP
		
		INFLATION = cb_send_info_message->inflation;
		INFLATION_TARGET = cb_send_info_message->inflation_target;

	FINISH_CB_SEND_INFO_MESSAGE_LOOP

	
    return 0;
}




/* \fn: int PCFirm_send_statistics()
 
* \brief: PCFirm send statistics.
 
 
* \timing: Monthly, the last day of the month.

 * \condition:

 
*\ pcfirm_send_statistics_message structure: <!-- (pcfirm_id, monthly_investments, physical_capital_price, real_monthly_investments) -->
 
 *\ 
 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int PCFirm_send_statistics()
{
	double real_monthly_investments = 0.0;
	
	if(PRICE > 0)
	real_monthly_investments = MONTHLY_REVENUE/PRICE;

	add_pcfirm_send_statistics_message(ID, MONTHLY_REVENUE, PRICE, real_monthly_investments);
	
	MONTHLY_REVENUE = 0.0;

	return 0;
}
