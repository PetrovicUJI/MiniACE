#include "../header.h"
#include "../my_library_header.h"

#include "../PCFirm_agent_header.h"


/* \fn: int PCFirm_initialization()
 
* \brief: PCFirm set initialization to zero.
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
 *\ init_pcfirm_to_gov message structure: <!-- (pcfirm_id, outstanding_shares) -->
 
 *\ pcfirm_price_info message structure: <!-- (pcfirm_id, pc_price, technology) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int PCFirm_initialization()
{
	add_init_pcfirm_to_gov_message(ID, OUTSTANDING_SHARES);
	
	add_pcfirm_price_info_message(ID, PRICE, TECHNOLOGY);
	
	INITIALIZATION = 0;

    return 0;
}
