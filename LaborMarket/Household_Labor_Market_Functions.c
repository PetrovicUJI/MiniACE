#include "../header.h"
#include "../my_library_header.h"

#include "../Household_agent_header.h"
#include "Household_Labor_Market_AUX_Headers.h"


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
	
	DAYS_OF_UNEMPLOYMENT = 1;
	

    return 0;
}

/* \fn: int Household_read_vacancies_and_send_job_applications()
 
* \brief: Household read vacancies and send job applications.
 
* \timing: Monthly.
* \condition: Given that they are employed
 
 
*\ 
*\ vacancies mesage structure: <!-- (employer_id, number_of_vacancies, wage_offer) -->
*\ job applications message   <!-- (employer_id, employee_id, wage) -->
*\ vacancy data structure <!-- (employer_id, number_of_vacancies, wage_offer) -->

* \authors: Marko Petrovic
* \history: 13.10.2017-Marko: First implementation.
*/
int Household_read_vacancies_and_send_job_applications()
{
	Vacancy_array  vacancy_list;
    init_Vacancy_array(&vacancy_list);
	
	START_VACANCIES_MESSAGE_LOOP
	
		add_Vacancy(&vacancy_list,
		vacancies_message->employer_id,
		vacancies_message->number_of_vacancies,
		vacancies_message->wage_offer);
		
    FINISH_VACANCIES_MESSAGE_LOOP
	
	int i;
	while(vacancy_list.size > MAX_NUMBER_OF_JOB_APPLICATIONS)
	{
		i = random_int(0, (vacancy_list.size-1));
		remove_Vacancy(&vacancy_list, i);
	}
	
	for(i = 0; i < (vacancy_list.size); i++)
    {
		if(vacancy_list.array[i].wage_offer >= RESERVATION_WAGE)
		{
			add_job_applications_message(vacancy_list.array[i].employer_id, ID, RESERVATION_WAGE);	
		}
	}
	
	free_Vacancy_array(&vacancy_list);

    return 0;
}

/* \fn: int Household_accept_job()
 
* \brief: Household accept job.
 
* \timing: Monthly.
* \condition: Given that they are unemployed
 
 
*\ message filter: a.id == m.employee_id 
*\ job offer message structure	 <!-- (employer_id, employee_id, wage) -->
*\ job offer data type structure <!-- (employer_id, employee_id, wage) -->

*\ 	job_acceptance message structure <!-- (employer_id, employee_id, wage) -->
 
* \authors: Marko Petrovic
* \history: 16.10.2017-Marko: First implementation.
*/
int Household_accept_job()
{
	/* Create a job offer dynamic array*/
    Job_offer_array job_offer_list;
    init_Job_offer_array(&job_offer_list);
	
	START_JOB_OFFER_MESSAGE_LOOP
	
		add_Job_offer(&job_offer_list,
		job_offer_message->employer_id,
		job_offer_message->employee_id,
		job_offer_message->wage);
		
    FINISH_JOB_OFFER_MESSAGE_LOOP
	
	/*Ranks job offers regarding the posted wage offer.*/
    qsort(job_offer_list.array, job_offer_list.size, sizeof(Job_offer),job_offer_list_rank_wage_offer_function);
	
	if(job_offer_list.array[0].employer_id != -1)
	{
		EMPLOYER_ID = job_offer_list.array[0].employer_id;
		WAGE = job_offer_list.array[0].wage;
		RESERVATION_WAGE = job_offer_list.array[0].wage;
		
		DAY_OF_MONTH_TO_ACT = DAY%20;
		DAY_OF_MONTH_TO_RECEIVE_INCOME = (DAY-1)%20;
		
		if(DAY_OF_MONTH_TO_ACT == 0) DAY_OF_MONTH_TO_ACT = 20;
		if(DAY_OF_MONTH_TO_RECEIVE_INCOME == 0) DAY_OF_MONTH_TO_RECEIVE_INCOME = 20;
		
		add_job_acceptance_message(EMPLOYER_ID, ID, WAGE);
	}
	else
	{
		// reservation wage decreases from 1-50% on the yearly base.
		RESERVATION_WAGE = max((1-((random_int(1,50)/100)/YEAR))*RESERVATION_WAGE, MIN_RESERVATION_WAGE);
	}
	
	/*Free the job offer dynamic array.*/
    free_Job_offer_array(&job_offer_list);
	
    return 0;
}

/* \fn: int Household_read_vacancies_and_send_job_applications_2()
 
* \brief: Household read vacancies and send job applications second round.
 
* \timing: Monthly.
* \condition: Given that they are employed
 
 
*\ 
*\ vacancies mesage structure: <!-- (employer_id, number_of_vacancies, wage_offer) -->
*\ job applications message   <!-- (employer_id, employee_id, wage) -->
*\ vacancy data structure <!-- (employer_id, number_of_vacancies, wage_offer) -->

* \authors: Marko Petrovic
* \history: 13.10.2017-Marko: First implementation.
*/
int Household_read_vacancies_and_send_job_applications_2()
{
	
	Vacancy_array  vacancy_list;
    init_Vacancy_array(&vacancy_list);
	
	START_VACANCIES_2_MESSAGE_LOOP
	
		add_Vacancy(&vacancy_list,
		vacancies_2_message->employer_id,
		vacancies_2_message->number_of_vacancies,
		vacancies_2_message->wage_offer);
		
    FINISH_VACANCIES_2_MESSAGE_LOOP
	
	int i;
	while(vacancy_list.size > MAX_NUMBER_OF_JOB_APPLICATIONS)
	{
		i = random_int(0, (vacancy_list.size-1));
		remove_Vacancy(&vacancy_list, i);
	}
	
	for(i = 0; i < (vacancy_list.size); i++)
    {
		if(vacancy_list.array[i].wage_offer >= RESERVATION_WAGE)
		{
			add_job_applications_2_message(vacancy_list.array[i].employer_id, ID, RESERVATION_WAGE);
		}
	}
	
	free_Vacancy_array(&vacancy_list);
	
    return 0;
}

/* \fn: int Household_accept_job_2()
 
* \brief: Household accept job second round.
 
* \timing: Monthly.
* \condition: Given that they are unemployed
 
 
*\ message filter: a.id == m.employee_id 
*\ job offer message structure	 <!-- (employer_id, employee_id, wage) -->
*\ job offer data type structure <!-- (employer_id, employee_id, wage) -->

*\ 	job_acceptance message structure <!-- (employer_id, employee_id, wage) -->
 
* \authors: Marko Petrovic
* \history: 16.10.2017-Marko: First implementation.
*/
int Household_accept_job_2()
{
	/* Create a job offer dynamic array*/
    Job_offer_array job_offer_list;
    init_Job_offer_array(&job_offer_list);
	
	START_JOB_OFFER_2_MESSAGE_LOOP
	
		add_Job_offer(&job_offer_list,
		job_offer_2_message->employer_id,
		job_offer_2_message->employee_id,
		job_offer_2_message->wage);
		
    FINISH_JOB_OFFER_2_MESSAGE_LOOP
	
	/*Ranks job offers regarding the posted wage offer.*/
    qsort(job_offer_list.array, job_offer_list.size, sizeof(Job_offer),job_offer_list_rank_wage_offer_function);
	
	if(job_offer_list.array[0].employer_id != -1)
	{
		EMPLOYER_ID = job_offer_list.array[0].employer_id;
		WAGE = job_offer_list.array[0].wage;
		RESERVATION_WAGE = job_offer_list.array[0].wage;
		
		DAY_OF_MONTH_TO_ACT = DAY%20;
		DAY_OF_MONTH_TO_RECEIVE_INCOME = (DAY-1)%20;
		
		if(DAY_OF_MONTH_TO_ACT == 0) DAY_OF_MONTH_TO_ACT = 20;
		if(DAY_OF_MONTH_TO_RECEIVE_INCOME == 0) DAY_OF_MONTH_TO_RECEIVE_INCOME = 20;
		
		add_job_acceptance_2_message(EMPLOYER_ID, ID, WAGE);
	}
	else
	{
		// reservation wage decreases from 1-50% on yearly base.
		RESERVATION_WAGE = max((1-((random_int(1,50)/100)/YEAR))*RESERVATION_WAGE, MIN_RESERVATION_WAGE);
	}
	
	/*Free the job offer dynamic array.*/
    free_Job_offer_array(&job_offer_list);

    return 0;
}

/* \fn: int Household_require_unemployment_benefits()
 
* \brief: Households require unemployment benefits.
 
* \timing:
 * \condition: If households found a new job
 
*\ unemployment_benefit message structure <!-- (gov_id, amount) -->
 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_require_unemployment_benefits()
{
	double amount = 0.0;
	
	amount = (UNEMPLOYMENT_BENEFIT/20)*DAYS_OF_UNEMPLOYMENT;
	
	add_unemployment_benefit_message(GOV_ID, amount);
	
	PAYMENT_ACCOUNT += amount;
	
	DAYS_OF_UNEMPLOYMENT = 0;

    return 0;
}

/* \fn: int Household_update_unemployment_period()
 
* \brief: Household update unemployment period.
 
* \timing:
 * \condition: If households is still unemployed.
 
 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_update_unemployment_period()
{
	DAYS_OF_UNEMPLOYMENT++;

    return 0;
}


/* \fn: int Household_receive_wage()
 
* \brief: Household receive wage.
 
* \timing: Monthly day of month to receive income
 * \condition: if employer_id != -1 and day_of_month_to_receive_income
 
  *\ wage_payment_message structure: <!-- (employee_id, wage) -->
  *\ message filter: a.id == m.employee_id
 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_receive_wage()
{
	START_WAGE_PAYMENT_MESSAGE_LOOP
	
		PAYMENT_ACCOUNT += wage_payment_message->wage;
		
    FINISH_WAGE_PAYMENT_MESSAGE_LOOP
	

    return 0;
}

/* \fn: int Household_require_transfers()
 
* \brief: Household require transfers.
 
* \timing: Monthly day of month to receive income
 * \condition: 
 
 *\ public_transfers message structure <!-- (gov_id, amount) -->
 
* \authors: Marko Petrovic
* \history: 27.10.2017-Marko: First implementation.
*/
int Household_require_transfers()
{
	add_public_transfers_message(GOV_ID, PUBLIC_TRANSFERS);
	
	PAYMENT_ACCOUNT += PUBLIC_TRANSFERS;

    return 0;
}

/* \fn: int Household_receive_dividends()
 
* \brief: Household receive dividends.

*\ dividend_payment message structure: 	<!-- (firm_id, dividend) -->

*\ Asset data structure: <!-- (asset_id, number_of_assets, book_value) -->
 
* \timing: Daily
 * \condition: 
 
 
* \authors: Marko Petrovic
* \history: 27.10.2017-Marko: First implementation.
*/
int Household_receive_dividends()
{
	START_DIVIDEND_PAYMENT_MESSAGE_LOOP
	
		for(int i = 0; i < ASSETS_LIST.size; i++)
		{
			if(ASSETS_LIST.array[i].asset_id == dividend_payment_message->firm_id)
			{
				PAYMENT_ACCOUNT += dividend_payment_message->dividend*ASSETS_LIST.array[i].number_of_assets;
				break;
			}
		}
	
    FINISH_DIVIDEND_PAYMENT_MESSAGE_LOOP
	
    return 0;
}




