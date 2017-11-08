#include "../header.h"
#include "../my_library_header.h"

#include "../Central_Bank_agent_header.h"



/* \fn: int CB_initialization()
 
* \brief: CB set initialization to zero.
 
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int CB_initialization()
{
	
	INITIALIZATION = 0;

    return 0;
}