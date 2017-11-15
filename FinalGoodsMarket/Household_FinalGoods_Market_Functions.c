#include "../header.h"
#include "../my_library_header.h"

#include "../Household_agent_header.h"

/* \fn: int Household_pay_tax_and_plan_consumption_budget()
 
* \brief: Household pay tax and plan consumption budget.
 
 *\ tax_payments_message structure:
<!-- (gov_id, tax_payment) -->
 
* \timing: 
 * \condition:
 

* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_pay_tax_and_plan_consumption_budget()
{
	double total_tax_payments = 0.0;
	
	CAPITAL_TAX_PAYMENT = DIVIDEND_INCOME*CAPITAL_TAX_RATE;
	INCOME_TAX_PAYMENT = INCOME*INCOME_TAX_RATE;
	
	total_tax_payments = INCOME_TAX_PAYMENT + CAPITAL_TAX_PAYMENT;
	
	add_tax_payments_message(GOV_ID, total_tax_payments);
	
	PAYMENT_ACCOUNT -= total_tax_payments;
	
	TOTAL_INCOME = INCOME + DIVIDEND_INCOME + PUBLIC_TRANSFERS;
	
	TOTAL_NET_INCOME = TOTAL_INCOME - total_tax_payments;
	
	TOTAL_ASSETS = CURRENT_ASSETS + NON_CURRENT_ASSETS + PAYMENT_ACCOUNT;
	TOTAL_LIABILITIES = CURRENT_LIABILITIES + NON_CURRENT_LIABILITIES;
	WEALTH = TOTAL_ASSETS - TOTAL_LIABILITIES;
	
	MONTHLY_CONSUMPTION_BUDGET = TOTAL_NET_INCOME + CARROL_INDEX*(WEALTH - WEALTH_TO_INCOME_RATIO_TARGET*TOTAL_NET_INCOME);
	
	MONTHLY_CONSUMPTION_BUDGET = max(MONTHLY_CONSUMPTION_BUDGET,0.5*TOTAL_NET_INCOME); 
	
	WEEK_COUNTER = 1;

	DIVIDEND_INCOME = 0.0;
	
	FILE *file1;
	char *filename;

	filename = malloc(120*sizeof(char));
	filename[0]=0;
	strcpy(filename, "its/Household_pay_tax_and_plan_consumption_budget.txt");      
	file1 = fopen(filename,"a");
	fprintf(file1,"\n %d %d %f %f %f %f",DAY, ID, TOTAL_ASSETS, CURRENT_ASSETS, NON_CURRENT_ASSETS, PAYMENT_ACCOUNT);
	fprintf(file1," %f %f %f %f",TOTAL_LIABILITIES, CURRENT_LIABILITIES, NON_CURRENT_LIABILITIES, WEALTH);
	fprintf(file1," %f %f %f %f",MONTHLY_CONSUMPTION_BUDGET, TOTAL_NET_INCOME, CARROL_INDEX, WEALTH_TO_INCOME_RATIO_TARGET);
	fclose(file1);
	free(filename);

    return 0;
}

/* \fn: int Household_buy_final_goods()
 
* \brief: Household buy final goods.
 
 
* \timing: Weekly
 * \condition:
 *\ final_goods_order_message structure	<!-- (buyer_id, budget) -->

* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_purchase_final_goods()
{
	if(WEEK_COUNTER > 4)
	printf("\n ERROR in function Household_purchase_final_goods: WEEK_COUNTER = %2.5f\n ", WEEK_COUNTER);
	
	//WEEKLY_BUDGET = MONTHLY_CONSUMPTION_BUDGET/(5-WEEK_COUNTER);
	
	WEEKLY_BUDGET = MONTHLY_CONSUMPTION_BUDGET;
	
	add_final_goods_order_message(ID, WEEKLY_BUDGET);

    return 0;
}

/* \fn: int Household_receive_final_goods()
 
* \brief: Household receive final goods.
 
 
* \timing: Weekly
 * \condition:
*\ filter message: a.id == m.buyer_id
*\ final_goods_delivery_to_buyers message structure:  <!-- (buyer_id, remaining_budget, costs, purchased_quantity) -->

* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_receive_final_goods()
{
	WEEKLY_CONSUMPTION_QUANTITY = 0.0;
	WEEKLY_CONSUMPTION_COSTS = 0.0;

	START_FINAL_GOODS_DELIVERY_TO_BUYERS_MESSAGE_LOOP
	
		// REMAINING_WEEKLY_BUDGET is redundand and should be removed!!!
		REMAINING_WEEKLY_BUDGET = final_goods_delivery_to_buyers_message->remaining_budget;
		WEEKLY_CONSUMPTION_QUANTITY = final_goods_delivery_to_buyers_message->purchased_quantity;
		WEEKLY_CONSUMPTION_COSTS = final_goods_delivery_to_buyers_message->costs;

    FINISH_FINAL_GOODS_DELIVERY_TO_BUYERS_MESSAGE_LOOP
	
	MONTHLY_CONSUMPTION_BUDGET -= WEEKLY_CONSUMPTION_COSTS;
	
	PAYMENT_ACCOUNT -= WEEKLY_CONSUMPTION_COSTS;
	
	WEEK_COUNTER++;

    return 0;
}







