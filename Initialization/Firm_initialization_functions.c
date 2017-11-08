#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"

int Idle()
{

    return 0;
}

/* \fn: int Firm_initialization()
 
* \brief: Firm set initialization to zero.
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
 *\ init_firm_to_gov message structure: <!-- (firm_id, outstanding_shares) -->
 
* \authors: Marko Petrovic
* \history: 27.09.2017-Marko: First implementation.
*/
int Firm_initialization()
{
	add_init_firm_to_gov_message(ID, OUTSTANDING_SHARES);
	
	INITIALIZATION = 0;

    return 0;
}

