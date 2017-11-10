#include "../header.h"
#include "../my_library_header.h"

#include "../Household_agent_header.h"

/* \fn: int Household_send_payments_to_bank()
 
* \brief: Household send payments to bank.
 
 
* \timing: Monthly on the last day of the month.

  *\ bank_account_update_message structure: 
 <!-- (bank_id, payment_account, delta_loan, delta_value_at_risk) -->
  filter: a.id==m.bank_id
 
* \authors: Marko Petrovic
* \history: 11.11.2017-Marko: First implementation.
*/
int Household_send_payments_to_bank()
{
	double delta_loan = 0.0;
	double delta_value_at_risk = 0.0;
	
	if(PAYMENT_ACCOUNT < 0)
	{
		delta_loan = fabs(PAYMENT_ACCOUNT);
		PAYMENT_ACCOUNT = 0;
		CURRENT_LIABILITIES += delta_loan;
		
		//default probabulity is 50%
		delta_value_at_risk = delta_loan*1.5;
		
		add_bank_account_update_message(BANK_ID, PAYMENT_ACCOUNT, delta_loan, delta_value_at_risk);
	}
	else
	{
		if(PAYMENT_ACCOUNT >= CURRENT_LIABILITIES)
		{
			PAYMENT_ACCOUNT -= CURRENT_LIABILITIES;
			
			delta_loan = (-1)*CURRENT_LIABILITIES;
			delta_value_at_risk = (-1.5)*CURRENT_LIABILITIES;

			CURRENT_LIABILITIES = 0.0;
			
			add_bank_account_update_message(BANK_ID, PAYMENT_ACCOUNT, delta_loan, delta_value_at_risk);
		}
		else
		{
			delta_loan = (-1)*PAYMENT_ACCOUNT;
			delta_value_at_risk = (-1.5)*PAYMENT_ACCOUNT;

			CURRENT_LIABILITIES -= PAYMENT_ACCOUNT;
			PAYMENT_ACCOUNT = 0.0;
			
			add_bank_account_update_message(BANK_ID, PAYMENT_ACCOUNT, delta_loan, delta_value_at_risk);
		}
	}
	
	TOTAL_ASSETS = CURRENT_ASSETS + NON_CURRENT_ASSETS + PAYMENT_ACCOUNT;
	TOTAL_LIABILITIES = CURRENT_LIABILITIES + NON_CURRENT_LIABILITIES;
	
	WEALTH = TOTAL_ASSETS - TOTAL_LIABILITIES;
	

    return 0;
}


