#include "../header.h"
#include "../my_library_header.h"

#include "../Government_agent_header.h"


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
		
    FINISH_PUBLIC_TRANSFERS_MESSAGE_LOOP
	
	
	START_BAILOUT_MESSAGE_LOOP
		
		PAYMENT_ACCOUNT -= bailout_message->bailout_amount;
		
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
		TAX_INCOME += tax_payments_message->tax_payment;
		
    FINISH_TAX_PAYMENTS_MESSAGE_LOOP

    return 0;
}

