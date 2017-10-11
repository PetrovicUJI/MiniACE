#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"

/* \fn: int Firm_send_loan_request()
 
* \brief: Firm send loan request.
 
 
* \timing: Monthly on the firm activation day.
 * \condition: if EXTERNAL_FINANCIAL_NEEDS > 0
 
 *\ loan request message structure: (agent_id, equity, total_debt, loan_amount)
 
* \authors: Marko Petrovic
* \history: 11.10.2017-Marko: First implementation.
*/
int Firm_send_loan_request()
{
	double total_debt = 0.0;
	
	total_debt = NON_CURRENT_LIABILITIES + CURRENT_LIABILITIES;
	
	add_loan_request_message(ID, EQUITY, total_debt, EXTERNAL_FINANCIAL_NEEDS);

    return 0;
}

/* \fn: int Firm_receive_loan()
 
* \brief: Firm receive new loan.
 
 
* \timing: Monthly on the firm activation day.
 * \condition: 
 
 \* 	new_loan_taken_message structure:
		(borrower_id, creditor_id, repayment_period_months, remaining_months, remaining_amount, instalment, 
		monthly_interest, loan_value_at_risk, loan_instalment_value_at_risk)
		
		 *\ message filter: a.id == m.borrower_id

 
* \authors: Marko Petrovic
* \history: 11.10.2017-Marko: First implementation.
*/
int Firm_receive_loan()
{	
	START_NEW_LOAN_TAKEN_MESSAGE_LOOP
	
		add_Loan(&LOANS, 
		new_loan_taken_message->creditor_id,
		new_loan_taken_message->repayment_period_months,
		new_loan_taken_message->remaining_months,
		new_loan_taken_message->remaining_amount,
		new_loan_taken_message->instalment,
		new_loan_taken_message->monthly_interest,
		new_loan_taken_message->loan_value_at_risk,
		new_loan_taken_message->loan_instalment_value_at_risk);
		
		PAYMENT_ACCOUNT += new_loan_taken_message->remaining_amount;
		NON_CURRENT_LIABILITIES += new_loan_taken_message->remaining_amount;
		
		EXTERNAL_FINANCIAL_NEEDS -= new_loan_taken_message->remaining_amount;

    FINISH_NEW_LOAN_TAKEN_MESSAGE_LOOP

    return 0;
}


