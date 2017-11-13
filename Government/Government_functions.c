#include "../header.h"
#include "../my_library_header.h"

#include "../Government_agent_header.h"




/* \fn: int Government_set_policy()
 
* \brief: Government set policy.
 
 
* \timing: Yearly


* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Government_set_policy()
{
	YEARLY_INCOME = YEARLY_TAX_INCOME;
	
	YEARLY_EXPENDITURE = YEARLY_PUBLIC_TRANSFERS_EXPENDITURE + YEARLY_BAILOUT_EXPENDITURE +
	YEARLY_BENEFITS_EXPENDITURE + YEARLY_INTEREST_PAYMENTS;

	BUDGET_BALANCE = YEARLY_INCOME - YEARLY_EXPENDITURE;
	
	DEBT_TO_GDP_RATIO = 0.0;
	
	if(YEARLY_GDP > 0)
	DEBT_TO_GDP_RATIO = TOTAL_LIABILITIES/YEARLY_GDP;
	
	VAT_RATE = 0.2;
	INCOME_TAX_RATE = 0.2;
	CAPITAL_TAX_RATE = 0.1;
	UNEMPLOYMENT_BENEFITS_RATE = 0.7;
	PUBLIC_TRANSFERS_RATE = 0.5;
	
	FILE *file1;
	char *filename;

	filename = malloc(120*sizeof(char));
	filename[0]=0;
	strcpy(filename, "its/Government_set_policy.txt");      
	file1 = fopen(filename,"a");
	fprintf(file1,"\n %d %d %f %f %f %f",DAY, ID, YEARLY_GDP, YEARLY_INCOME, YEARLY_EXPENDITURE, BUDGET_BALANCE);
	fprintf(file1,"\n %f",DEBT_TO_GDP_RATIO, TOTAL_LIABILITIES, MEAN_WAGE);
	fclose(file1);
	free(filename);
	
	YEARLY_GDP = 0.0;
	
	YEARLY_PUBLIC_TRANSFERS_EXPENDITURE = 0.0;
	YEARLY_BAILOUT_EXPENDITURE = 0.0;
	YEARLY_BENEFITS_EXPENDITURE = 0.0;
	YEARLY_INTEREST_PAYMENTS = 0.0;
	
	YEARLY_TAX_INCOME = 0.0;
	

    return 0;
}






/* \fn: int Government_pays_unemployment_benefits()
 
* \brief: Government pays unemployment benefits.
 
 
* \timing: Daily
*\ unemployment_benefit message structure <!-- (gov_id, amount) -->
 

 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Government_pays_unemployment_benefits()
{
	START_UNEMPLOYMENT_BENEFIT_MESSAGE_LOOP
		
		PAYMENT_ACCOUNT -= unemployment_benefit_message->amount;
		
		MONTHLY_BENEFITS_EXPENDITURE += unemployment_benefit_message->amount;
		YEARLY_BENEFITS_EXPENDITURE += unemployment_benefit_message->amount;

		
    FINISH_UNEMPLOYMENT_BENEFIT_MESSAGE_LOOP
	

    return 0;
}

/* \fn: int Government_pays_transfers()
 
* \brief: Government pays transfers.
 
 
* \timing: Daily
*\ public_transfers message structure <!-- (gov_id, amount) -->

*\ bailout message structure: <!-- (gov_id, agent_id, bailout_amount) -->
 

 
* \authors: Marko Petrovic
* \history: 27.10.2017-Marko: First implementation.
*/
int Government_pays_transfers()
{
	START_PUBLIC_TRANSFERS_MESSAGE_LOOP
		
		PAYMENT_ACCOUNT -= public_transfers_message->amount;
		
		MONTHLY_PUBLIC_TRANSFERS_EXPENDITURE += public_transfers_message->amount;
		YEARLY_PUBLIC_TRANSFERS_EXPENDITURE += public_transfers_message->amount;

		
    FINISH_PUBLIC_TRANSFERS_MESSAGE_LOOP
	
	
	START_BAILOUT_MESSAGE_LOOP
		
		PAYMENT_ACCOUNT -= bailout_message->bailout_amount;
		
		MONTHLY_BAILOUT_EXPENDITURE += bailout_message->bailout_amount;
		YEARLY_BAILOUT_EXPENDITURE += bailout_message->bailout_amount;

		
    FINISH_BAILOUT_MESSAGE_LOOP

    return 0;
}

/* \fn: int Government_collect_tax()
 
* \brief: Government collect tax.
 
 
* \timing: Daily

*\ tax_payments_message structure:
<!-- (gov_id, tax_payment) -->

 
* \authors: Marko Petrovic
* \history: 28.10.2017-Marko: First implementation.
*/
int Government_collect_tax()
{
	START_TAX_PAYMENTS_MESSAGE_LOOP
		
		PAYMENT_ACCOUNT += tax_payments_message->tax_payment;
		
		MONTHLY_TAX_INCOME += tax_payments_message->tax_payment;
		YEARLY_TAX_INCOME += tax_payments_message->tax_payment;
			
    FINISH_TAX_PAYMENTS_MESSAGE_LOOP

    return 0;
}

