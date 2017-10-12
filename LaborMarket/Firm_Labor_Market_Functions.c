#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"

/* \fn: int Firm_send_redundancies_or_open_new_vacancies()
 
* \brief: Firm send redundancies or open new vacancies.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 12.10.2017-Marko: First implementation.
*/
int Firm_send_redundancies_or_open_new_vacancies()
{
	VACANCIES = 0.0;
	
	
	if(LABOR_REQUIREMENT >= EMPLOYEES.size)
	{
		VACANCIES = LABOR_REQUIREMENT-EMPLOYEES.size;
	}
	else
	{
		
	}
	


    return 0;
}

int Firm_hire_new_workers()
{	
	

    return 0;
}