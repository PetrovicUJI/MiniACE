#include "../header.h"
#include "../my_library_header.h"

#include "../Labor_Market_Clearing_agent_header.h"

/* \fn: int LMC_collect_vacancies_and_job_applications()
 
* \brief: Labor Market Clearing collects all posted vacancies and job appplications.
 
 
* \timing: .
 * \condition:
 
* \authors: Marko Petrovic
* \history: 13.10.2017-Marko: First implementation.
*/
int LMC_collect_vacancies_and_job_applications()
{
	/* Make sure that arrays are clear.*/
    if (VACANCY_LIST.size > 0) 
	reset_Vacancy_array(&VACANCY_LIST);

	if (JOB_APPLICATION_LIST.size > 0)
	reset_Job_application_array(&JOB_APPLICATION_LIST);
	
	//vacancies_message(employer_id, number_of_vacancies, wage_offer)
	//Vacancy_data_type(employer_id, number_of_vacancies, wage_offer)
	START_VACANCIES_MESSAGE_LOOP
		
		add_Vacancy(&VACANCY_LIST, 
		vacancies_message->employer_id,
		vacancies_message->number_of_vacancies,
		vacancies_message->wage_offer);
		
    FINISH_VACANCIES_MESSAGE_LOOP
	
	// sort the list randomly
	Vacancy_array vacancy_list_sorted;
	init_Vacancy_array(&vacancy_list_sorted);
	
	int i = 0;
	while(VACANCY_LIST.size > 0)
	{
		i = random_int(0,(VACANCY_LIST.size-1));
		
		add_Vacancy(&vacancy_list_sorted, 
		VACANCY_LIST.array[i].employer_id, 
		VACANCY_LIST.array[i].number_of_vacancies,
		VACANCY_LIST.array[i].wage_offer);
		
		remove_Vacancy(&VACANCY_LIST,i);
	}
	
	if(VACANCY_LIST.size > 0)
	
	
	
	
	
	// job_applications_message(employee_id, reservation_wage)
	// Job_application_data_type(employee_id, reservation_wage)
	START_JOB_APPLICATIONS_MESSAGE_LOOP
		
		add_Job_application(&JOB_APPLICATION_LIST, 
		job_applications_message->employee_id,
		job_applications_message->reservation_wage);
		
    FINISH_JOB_APPLICATIONS_MESSAGE_LOOP
	
    return 0;
}




/* \fn: int LMC_match_labor_demand_and_supply()
 
* \brief: Labor Market Clearing match labor demand and labor supply.
 
 
* \timing: .
 * \condition:
 
* \authors: Marko Petrovic
* \history: 13.10.2017-Marko: First implementation.
*/
int LMC_match_labor_demand_and_supply()
{
	Job_application_array job_application_list_sorted;
	init_Job_application_array(&job_application_list_sorted);
	
	int i = 0;
	int j = 0;
	int k = 0;
	while(JOB_APPLICATION_LIST.size > 0)
	{
		i = random_int(0,(JOB_APPLICATION_LIST.size-1));
		
		for(j = 0; j < min(VACANCY_LIST.size, NUMBER_OF_FIRMS2APPLY4JOB); j++)
		{
			j = random_int(0,(VACANCY_LIST.size-1));
		}
		
		add_Job_application(&job_application_list_sorted, 
		JOB_APPLICATION_LIST.array[i].employee_id, 
		JOB_APPLICATION_LIST.array[i].reservation_wage);
		
		
		
		
		remove_Job_application(&JOB_APPLICATION_LIST,i);
	}
	
	
	

	
    return 0;
}











