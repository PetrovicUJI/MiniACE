#include "../header.h"
#include "../my_library_header.h"

#include "../Household_agent_header.h"

/* \fn: int Household_send_unemployment_benefit_request()
 
* \brief: Households count days of being unemployed and ask for unemployment benefits every month (20 days).
 
* \timing: .
 * \condition:
 
* \authors: Marko Petrovic
* \history: 13.10.2017-Marko: First implementation.
*/
int Household_send_unemployment_benefit_request()
{
	if(DAYS_OF_UNEMPLOYMENT%20 != 0)
	{
		DAYS_OF_UNEMPLOYMENT++;
	}
	else
	{
		DAYS_OF_UNEMPLOYMENT++;
		// send message to the government
	}
	
	return 0;
}



int Household_send_job_applications()
{
	

    return 0;
}



/* \fn: int Household_get_fired()
 
* \brief: Households read firing message.
 
* \timing: Monthly.
 * \condition: Given that they are employed
 
 
 *\ message filter: a.id == m.employee_id 
 *\ redundancies mesage structure: <!-- (employer_id, employee_id) -->
 
* \authors: Marko Petrovic
* \history: 13.10.2017-Marko: First implementation.
*/
int Household_get_fired()
{
	START_REDUNDANCIES_MESSAGE_LOOP
	
		EMPLOYER_ID = redundancies_message->employer_id;
		
    FINISH_REDUNDANCIES_MESSAGE_LOOP
	
	DAYS_OF_UNEMPLOYMENT = 0;
	

    return 0;
}