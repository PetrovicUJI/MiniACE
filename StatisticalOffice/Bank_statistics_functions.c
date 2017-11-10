#include "../header.h"
#include "../my_library_header.h"

#include "../Bank_agent_header.h"


/* \fn: int Bank_send_info()
 
* \brief: Bank send info to households.
 
  *\ enterprise_to_hh_info message structure: 	<!-- (enterprise_id, share_book_value) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Bank_send_info()
{
	double share_book_value = 0.0;
	
	if(OUTSTANDING_SHARES <= 0)
	printf("\n ERROR in function Bank_send_info: OUTSTANDING_SHARES = %2.5f\n ", OUTSTANDING_SHARES);
	
	if(EQUITY > 0)
	share_book_value = EQUITY/OUTSTANDING_SHARES;

	add_enterprise_to_hh_info_message(ID, share_book_value);


    return 0;
}

/* \fn: int Bank_send_statistics()
 
* \brief: Bank send statistics.
 
 
* \timing: Monthly, the last day of the month.

 * \condition:

 
 *\ bank_send_statistics_message structure: <!-- (bank_id, value_at_risk) -->
 
* \authors: Marko Petrovic
* \history: 10.11.2017-Marko: First implementation.
*/
int Bank_send_statistics()
{

	add_bank_send_statistics_message(ID, VALUE_AT_RISK);
	

	return 0;
}