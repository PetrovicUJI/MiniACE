#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"

/* \fn: int Firm_send_redundancies_or_open_new_vacancies()
 
* \brief: Firm send redundancies or open new vacancies.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
  *\ redundancies mesage structure: <!-- (employer_id, employee_id) -->
  
  *\ vacancies mesage structure: <!-- (employer_id, number_of_vacancies, wage_offer) -->

* \authors: Marko Petrovic
* \history: 13.10.2017-Marko: First implementation.
*/
int Firm_send_redundancies_or_open_new_vacancies()
{
	if(LABOR_REQUIREMENT < 0)
	printf("\n ERROR in function Firm_send_redundancies_or_open_new_vacancies: LABOR_REQUIREMENT = %2.5f\n ", LABOR_REQUIREMENT);


	if(LABOR_REQUIREMENT > EMPLOYEES.size)
	{
		add_vacancies_message(ID, LABOR_REQUIREMENT - EMPLOYEES.size, WAGE_OFFER);
	}
	
	if(LABOR_REQUIREMENT < EMPLOYEES.size)
	{
		int i = 0;
		while(EMPLOYEES.size == LABOR_REQUIREMENT)
		{
			i = random_int(0,(EMPLOYEES.size-1));
			add_redundancies_message(-1, EMPLOYEES.array[i].id);
			remove_Employee(&EMPLOYEES,i);
		}
	}
	


    return 0;
}

int Firm_hire_new_workers()
{	
	

    return 0;
}