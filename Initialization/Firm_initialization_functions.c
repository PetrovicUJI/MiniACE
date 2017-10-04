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
 
* \authors: Marko Petrovic
* \history: 27.09.2017-Marko: First implementation.
*/
int Firm_initialization()
{
	
	INITIALIZATION = 0;

    return 0;
}

