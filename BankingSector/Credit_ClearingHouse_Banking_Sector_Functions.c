#include "../header.h"
#include "../my_library_header.h"

#include "../Credit_ClearingHouse_agent_header.h"


int CCH_idle()
{

    return 0;
}



/* \fn: int CCH_receive_loan_requests()
 
* \brief: Credit clearing house receive loan requests.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 11.10.2017-Marko: First implementation.
*/
int CCH_collect_orders()
{
	/* Make sure that order arrays are clear.*/
    if (LOAN_REQUESTS_LIST.size > 0) reset_Loan_request_array(&LOAN_REQUESTS_LIST);
	if (LOAN_OFFERS_LIST.size > 0) reset_Loan_offer_array(&LOAN_OFFERS_LIST);
	

	
	int agent_id = 0;
	double equity = 0.0;
	double total_debt = 0.0;
	double loan_amount = 0.0;
	double value_at_risk = 0.0;
	

	//loan_request_message(agent_id, equity, total_debt, loan_amount)
	START_LOAN_REQUEST_MESSAGE_LOOP
	
		agent_id = loan_request_message->agent_id;
		equity = loan_request_message->equity;
		total_debt = loan_request_message->total_debt;
		loan_amount = loan_request_message->loan_amount;
		
		add_Loan_request(&LOAN_REQUESTS_LIST, agent_id, equity, total_debt, loan_amount);
		
    FINISH_LOAN_REQUEST_MESSAGE_LOOP
	
	//loan_offer_message(agent_id, equity, value_at_risk)
	START_LOAN_OFFER_MESSAGE_LOOP

		agent_id = loan_offer_message->agent_id;
		equity = loan_offer_message->equity;
		value_at_risk = loan_offer_message->value_at_risk;
		
		add_Loan_offer(&LOAN_OFFERS_LIST, agent_id, equity, value_at_risk);
		
    FINISH_LOAN_OFFER_MESSAGE_LOOP

    return 0;
}

/* \fn: int CCH_allocate loans()
 
* \brief: Credit clearing house allocate loans.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 11.10.2017-Marko: First implementation.
*/
int CCH_allocate_loans()
{
	if(LOAN_REQUESTS_LIST.size == 0 || LOAN_OFFERS_LIST.size == 0)
	{
		return 0;
	}
	
	int i = 0;
	
	int borrower_id = 0;
	double borrower_equity = 0.0;
	double total_debt = 0.0;
	double loan_requested = 0.0;
	double bankruptcy_prob = 0.0;
	double weighted_loan_requested = 0.0;
	double interest_rate = 0.0;
	
	double creditor_id = 0.0;
	double value_at_risk = 0.0;
	double lender_equity = 0.0;
	
	double credit_allowed = 0.0;
	
	while(LOAN_REQUESTS_LIST.size > 0)
	{
		i = random_int(0,(LOAN_REQUESTS_LIST.size-1));
		
		borrower_id = LOAN_REQUESTS_LIST.array[i].agent_id;
		borrower_equity = LOAN_REQUESTS_LIST.array[i].equity;
		total_debt = LOAN_REQUESTS_LIST.array[i].total_debt;
		loan_requested = LOAN_REQUESTS_LIST.array[i].loan_amount;
		
		if(total_debt+loan_requested+borrower_equity <= 0)
		printf("\n ERROR in function CCH_allocate_loans: total_debt+loan_requested+borrower_equity = %2.5f\n ", total_debt+loan_requested+borrower_equity);
		
		bankruptcy_prob = 2.5*pow((total_debt+loan_requested)/(total_debt+loan_requested+borrower_equity),3);
		
		weighted_loan_requested = bankruptcy_prob*loan_requested;
		
		interest_rate = CB_INTEREST_RATE + bankruptcy_prob*(double)rand()/RAND_MAX;
		
		creditor_id = LOAN_OFFERS_LIST.array[0].agent_id;
		value_at_risk = LOAN_OFFERS_LIST.array[0].value_at_risk;
		lender_equity = LOAN_OFFERS_LIST.array[0].equity;
		
		if((value_at_risk + weighted_loan_requested) <= ALPHA*lender_equity)
		{
			credit_allowed = loan_requested;
			if (credit_allowed<0)
			printf("\n ERROR in function CCH_allocate_loans: credit_allowed = %2.5f\n ", credit_allowed);
			
			/* 	new_loan_taken_message structure:
				(borrower_id, creditor_id, repayment_period_months, remaining_months, remaining_amount, instalment, 
				monthly_interest, loan_value_at_risk, loan_instalment_value_at_risk)
			*/
			if(LOAN_REPAYMENT_PERIOD_MONTHS > 0)
			{
				add_new_loan_taken_message(borrower_id, creditor_id, LOAN_REPAYMENT_PERIOD_MONTHS, LOAN_REPAYMENT_PERIOD_MONTHS, credit_allowed,
									   credit_allowed/LOAN_REPAYMENT_PERIOD_MONTHS, interest_rate/12, bankruptcy_prob*credit_allowed, 
									   bankruptcy_prob*credit_allowed/LOAN_REPAYMENT_PERIOD_MONTHS);
									   
				// new loan given message structure: creditor_id, credit_allowed, loan_value_at_risk
				
				add_new_loan_given_message(creditor_id, credit_allowed, bankruptcy_prob*credit_allowed);
				
			}
			else
			{
				printf("\n ERROR in function CCH_allocate_loans: LOAN_REPAYMENT_PERIOD_MONTH = %2.5f\n ", LOAN_REPAYMENT_PERIOD_MONTHS);
			}

			remove_Loan_request(&LOAN_REQUESTS_LIST,i);
		}
		else 
		{
			credit_allowed = max(0,(ALPHA*lender_equity - value_at_risk)/bankruptcy_prob);  
			if (credit_allowed<0)
			printf("\n ERROR in function bank_decide_credit_condition: credit_allowed = %2.5f\n ", credit_allowed);
		
			/* 	new_loan_taken_message structure:
			(borrower_id, creditor_id, repayment_period_months, remaining_months, remaining_amount, instalment, 
			monthly_interest, loan_value_at_risk, loan_instalment_value_at_risk)
			*/
			if(LOAN_REPAYMENT_PERIOD_MONTHS > 0)
			{
				add_new_loan_taken_message(borrower_id, creditor_id, LOAN_REPAYMENT_PERIOD_MONTHS, LOAN_REPAYMENT_PERIOD_MONTHS, credit_allowed,
									   credit_allowed/LOAN_REPAYMENT_PERIOD_MONTHS, interest_rate/12, bankruptcy_prob*credit_allowed, 
									   bankruptcy_prob*credit_allowed/LOAN_REPAYMENT_PERIOD_MONTHS);
									   
				// new loan given message structure: creditor_id, credit_allowed, loan_value_at_risk
				
				add_new_loan_given_message(creditor_id, credit_allowed, bankruptcy_prob*credit_allowed);
				
			}
			else
			{
				printf("\n ERROR in function CCH_allocate_loans: LOAN_REPAYMENT_PERIOD_MONTH = %2.5f\n ", LOAN_REPAYMENT_PERIOD_MONTHS);
			}
		
			break;
		}
	}

    return 0;
}