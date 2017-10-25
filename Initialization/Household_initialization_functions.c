#include "../header.h"
#include "../my_library_header.h"

#include "../Household_agent_header.h"



/* \fn: int Household_initialization()
 
* \brief: Household set initialization to zero.
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_initialization()
{
	
	INITIALIZATION = 0;

    return 0;
}