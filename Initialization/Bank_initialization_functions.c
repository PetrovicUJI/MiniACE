#include "../header.h"
#include "../my_library_header.h"

#include "../Bank_agent_header.h"



/* \fn: int Bank_initialization()
 
* \brief: Bank set initialization to zero.
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
  *\ init_bank_to_gov message structure: <!-- (bank_id, outstanding_shares) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Bank_initialization()
{
	add_init_bank_to_gov_message(ID, OUTSTANDING_SHARES);
	
	INITIALIZATION = 0;

    return 0;
}