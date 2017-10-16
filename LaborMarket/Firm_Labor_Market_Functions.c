#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"

/* \fn: int Firm_calculate_labor_demand()
 
* \brief: Firms decide the number of workers to be hired or fired.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 

* \authors: Marko Petrovic
* \history: 16.10.2017-Marko: First implementation.
*/
int Firm_calculate_labor_demand()
{	
	if(LABOR_REQUIREMENT < 0)
	printf("\n ERROR in function Firm_calculate_labor_demand: LABOR_REQUIREMENT = %2.5f\n ", LABOR_REQUIREMENT);

	DELTA_LABOR_REQUIREMENT = LABOR_REQUIREMENT - EMPLOYEES.size;

    return 0;
}



/* \fn: int Firm_send_redundancies()
 
* \brief: Firm send redundancies.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
  *\ redundancies mesage structure: <!-- (employer_id, employee_id) -->
  

* \authors: Marko Petrovic
* \history: 13.10.2017-Marko: First implementation.
*/
int Firm_send_redundancies()
{
	
	int i = 0;
	while(EMPLOYEES.size > LABOR_REQUIREMENT)
	{
		i = random_int(0,(EMPLOYEES.size-1));
		add_redundancies_message(-1, EMPLOYEES.array[i].id);
		remove_Employee(&EMPLOYEES,i);
	}
	

    return 0;
}

/* \fn: int Firm_post_vacancies()
 
* \brief: Firm open new vacancies.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
  
  *\ vacancies mesage structure: <!-- (employer_id, number_of_vacancies, wage_offer) -->

* \authors: Marko Petrovic
* \history: 16.10.2017-Marko: First implementation.
*/
int Firm_post_vacancies()
{
	
	add_vacancies_message(ID, LABOR_REQUIREMENT - EMPLOYEES.size, WAGE_OFFER);

    return 0;
}

/* \fn: int Firm_post_vacancies_2()
 
* \brief: Firm open new vacancies.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
  
  *\ vacancies_2 mesage structure: <!-- (employer_id, number_of_vacancies, wage_offer) -->

* \authors: Marko Petrovic
* \history: 16.10.2017-Marko: First implementation.
*/
int Firm_post_vacancies_2()
{
	
	add_vacancies_2_message(ID, LABOR_REQUIREMENT - EMPLOYEES.size, WAGE_OFFER);

    return 0;
}


/* \fn: int Firm_read_job_applications_and_send_job_offers()
 
* \brief: Firm read job applications and send job offers.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
  
*\ job applications message structure   <!-- (employer_id, employee_id, reservation_wage) -->

*\ job application data structure <!-- (employee_id, reservation_wage) -->

*\ job_offer_message structure<!-- (employer_id, employee_id, wage) -->

 *\ filter: a.id == m.employer_id

* \authors: Marko Petrovic
* \history: 16.10.2017-Marko: First implementation.
*/

int Firm_read_job_applications_and_send_job_offers()
{	
	Job_application_array  job_application_list;
    init_Job_application_array(&job_application_list);
	
	START_JOB_APPLICATIONS_MESSAGE_LOOP
	
		add_Job_application(&job_application_list,
		job_applications_message->employee_id,
		job_applications_message->reservation_wage);
		
    FINISH_JOB_APPLICATIONS_MESSAGE_LOOP
	

	int i;
	while(job_application_list.size > DELTA_LABOR_REQUIREMENT)
	{
		i = random_int(0, (job_application_list.size-1));
		
		add_job_offer_message(-1, job_application_list.array[i].employee_id, 0);
		
		remove_Job_application(&job_application_list, i);
	}
	
	for(i = 0; i < (job_application_list.size); i++)
    {
        add_job_offer_message(ID, job_application_list.array[i].employee_id, WAGE_OFFER);
    }
	
	free_Job_application_array(&job_application_list);

    return 0;
}


/* \fn: int Firm_hire_new_workers()
 
* \brief: Firm hire new workers. Read job acceptance message.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
  
*\ 	job_acceptance message structure <!-- (employer_id, employee_id, wage) -->

*\ Employee data structure <!-- (id, wage) -->

 *\ filter: a.id == m.employer_id

* \authors: Marko Petrovic
* \history: 16.10.2017-Marko: First implementation.
*/
int Firm_hire_new_workers()
{	
	START_JOB_ACCEPTANCE_MESSAGE_LOOP
	
		add_Employee(&EMPLOYEES, 
		job_acceptance_message->employee_id,
		job_acceptance_message->wage);
		
		DELTA_LABOR_REQUIREMENT--;
		
    FINISH_JOB_ACCEPTANCE_MESSAGE_LOOP
	

    return 0;
}

int Firm_read_job_applications_and_send_job_offers_2()
{	
	

    return 0;
}

int Firm_hire_new_workers_2()
{	
	

    return 0;
}









