#include "../header.h"
#include "../my_library_header.h"

#include "../Government_agent_header.h"



/* \fn: int Government_initialization()
 
* \brief: Government set initialization to zero.

*\ hh_to_gov_ini_message structure: <!-- (hh_id) -->
 
*\ firm_to_gov_ini_message structure: <!-- (firm_id, outstanding_shares) -->
 
*\ bank_to_gov_ini_message structure: <!-- (bank_id, outstanding_shares) -->

*\ pcfirm_to_gov_ini_message structure: <!-- (pcfirm_id, outstanding_shares) -->

 *\ cb_to_gov_ini_message structure: 	<!-- (cb_id) -->


*\ 	gov_to_hh_init_shares message structure <!-- (hh_id, enterprise_id, shares, share_book_value) -->
 
 
 *\ Enterprise_initialization data type structure <!-- (enterprise_id, outstanding_shares, share_book_value) -->
 
 *\ gov_init_labor_message structure: <!-- (hh_id, employer_id, activation_day, wage) -->
 filters: households: a.id == m.hh_id;    firms:  a.id == m.employer_id
 
*\gov_init_balance_sheets message structure:
*\ <!-- (agent_id, current_assets, non_current_assets, payment_account, current_liabilities, non_current_liabilities) -->
 filter: a.id == m.agent_id
 
*\ pcfirm_price_info message structure: <!-- (pcfirm_id, pc_price, technology) -->
 
* \timing: First iteration only when initialization is needed.
 * \condition: if INITIALIZATION == 1
 
* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Government_initialization()
{
		/////////////////////////////////////////////////////////
	
	// Make file to be printed
	
	
	FILE *file1;
	char *filename;

	if(DEBUG_MODE)
	{
		filename = malloc(120*sizeof(char));
		filename[0]=0;
		strcpy(filename, "its/DAILY_SOLD_QUANTITY.txt");      
		file1 = fopen(filename,"a");
		fprintf(file1,"%s %s","DAILY_SOLD_QUANTITY", "DAILY_MARKET_TURNOVER");
		fclose(file1);
		free(filename);
	}

	
	filename = malloc(120*sizeof(char));
	filename[0]=0;
	strcpy(filename, "its/Government_set_policy.txt");      
	file1 = fopen(filename,"a");
	fprintf(file1,"%s %s %s %s %s %s","DAY", "ID", "YEARLY_GDP", "YEARLY_INCOME", "YEARLY_EXPENDITURE", "BUDGET_BALANCE");
	fprintf(file1," %s %s %s","DEBT_TO_GDP_RATIO", "TOTAL_LIABILITIES", "MEAN_WAGE");
	fclose(file1);
	free(filename);

	filename = malloc(120*sizeof(char));
	filename[0]=0;
	strcpy(filename, "its/Household_receive_initialization.txt");      
	file1 = fopen(filename,"a");
	fprintf(file1,"%s %s %s %s %s %s","DAY", "ID", "TOTAL_ASSETS", "CURRENT_ASSETS", "NON_CURRENT_ASSETS", "PAYMENT_ACCOUNT");
	fprintf(file1," %s %s %s %s %s","TOTAL_LIABILITIES", "CURRENT_LIABILITIES", "NON_CURRENT_LIABILITIES", "WEALTH", "number_of_assets");
	fclose(file1);
	free(filename);
	
	if(DEBUG_MODE)
	{
		filename = malloc(120*sizeof(char));
		filename[0]=0;
		strcpy(filename, "its/Firm_stat.txt");      
		file1 = fopen(filename,"a");
		fprintf(file1,"%s %s %s %s %s %s %s","DAY", "ID", "TOTAL_ASSETS", "CURRENT_ASSETS", "INVENTORIES", "NON_CURRENT_ASSETS", "PAYMENT_ACCOUNT");
		fprintf(file1," %s %s %s %s","TOTAL_LIABILITIES", "CURRENT_LIABILITIES", "NON_CURRENT_LIABILITIES", "EQUITY");
		fclose(file1);
		free(filename);
	}

	
	filename = malloc(120*sizeof(char));
	filename[0]=0;
	strcpy(filename, "its/Household_pay_tax_and_plan_consumption_budget.txt");      
	file1 = fopen(filename,"a");
	fprintf(file1,"%s %s %s %s %s %s","DAY", "ID", "TOTAL_ASSETS", "CURRENT_ASSETS", "NON_CURRENT_ASSETS", "PAYMENT_ACCOUNT");
	fprintf(file1," %s %s %s %s","TOTAL_LIABILITIES", "CURRENT_LIABILITIES", "NON_CURRENT_LIABILITIES", "WEALTH");
	fprintf(file1," %s %s %s %s","MONTHLY_CONSUMPTION_BUDGET", "TOTAL_NET_INCOME", "CARROL_INDEX", "WEALTH_TO_INCOME_RATIO_TARGET");
	fclose(file1);
	free(filename);
	
	filename = malloc(120*sizeof(char));
	filename[0]=0;
	strcpy(filename, "its/Statistical_Office_end_of_the_month_statistics.txt");      
	file1 = fopen(filename,"a");
	fprintf(file1," %s %s %s %s %s %s","DAY", "ID", "NOMINAL_MONTHLY_GDP", "REAL_MONTHLY_GDP", "PRICE_LEVEL", "PRICE_GROWTH");
	fprintf(file1," %s %s %s","AVERAGE_WAGE", "UNEMPLOYMENT_RATE", "num_of_bankruptcies");
	fprintf(file1," %s %s %s","agregate_capital_stock", "agregate_inventory_stock", "agregate_production");
	fprintf(file1," %s %s %s %s","AGGREGATE_VALUE_AT_RISK", "monthly_investments", "physical_capital_price", "real_monthly_investments");
	fclose(file1);
	free(filename);
	
	
	filename = malloc(120*sizeof(char));
	filename[0]=0;
	strcpy(filename, "its/Firm_balance_sheet.txt");      
	file1 = fopen(filename,"w");
	fprintf(file1,"%s %s %s %s %s %s %s","DAY", "ID", "TOTAL_ASSETS", "CURRENT_ASSETS", "INVENTORIES", "NON_CURRENT_ASSETS", "PAYMENT_ACCOUNT");
	fprintf(file1," %s %s %s %s","TOTAL_LIABILITIES", "CURRENT_LIABILITIES", "NON_CURRENT_LIABILITIES", "EQUITY");
	fprintf(file1," %s %s","PHYSICAL_CAPITAL", "ACTIVE");
	fclose(file1);
	free(filename);
	
	filename = malloc(120*sizeof(char));
	filename[0]=0;
	strcpy(filename, "its/Firm_income_statement.txt");      
	file1 = fopen(filename,"a");
	fprintf(file1,"%s %s %s %s %s %s %s","DAY", "ID", "MONTHLY_REVENUE", "VAT_PAYMENT", "EBT", "MONTHLY_WAGE_PAYMENT", "TOTAL_FINANCIAL_PAYMENT");
	fprintf(file1," %s %s %s %s","PHYSICAL_CAPITAL_DEPRECIATION_COST", "DIVIDEND_PAYMENT", "CAPITAL_TAX_PAYMENT", "NET_EARNINGS");
	fclose(file1);
	free(filename);
	
	/////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	Enterprise_initialization_array firm_list;
	init_Enterprise_initialization_array(&firm_list);
	
	Enterprise_initialization_array pcfirm_list;
	init_Enterprise_initialization_array(&pcfirm_list);
	
	Enterprise_initialization_array bank_list;
	init_Enterprise_initialization_array(&bank_list);
	
	int_array hh_list;
	init_int_array(&hh_list);
	
	int number_of_firms = 0;
	START_FIRM_TO_GOV_INI_MESSAGE_LOOP
			
			number_of_firms++;
		
	FINISH_FIRM_TO_GOV_INI_MESSAGE_LOOP
	
	START_HH_TO_GOV_INI_MESSAGE_LOOP
	
		add_int(&hh_list, hh_to_gov_ini_message->hh_id);
	
    FINISH_HH_TO_GOV_INI_MESSAGE_LOOP
	
	double physical_capital_price = 0.0;
	START_PCFIRM_PRICE_INFO_MESSAGE_LOOP
	
		physical_capital_price = pcfirm_price_info_message->pc_price;
	
    FINISH_PCFIRM_PRICE_INFO_MESSAGE_LOOP
	
	int cb_id;
	START_CB_TO_GOV_INI_MESSAGE_LOOP
	
		cb_id = cb_to_gov_ini_message->cb_id;
	
    FINISH_CB_TO_GOV_INI_MESSAGE_LOOP
	

	// initialize firm balance sheet
	
	double total_production = MEAN_WAGE*hh_list.size; // given prices equal to 1.
	double firm_production = total_production/number_of_firms;
	double firm_capital = firm_production; // given productivity = 1
	double firm_inventories = firm_production;
	double firm_endowment_payment_account = MEAN_WAGE*hh_list.size/number_of_firms;
	
	double firm_equity = firm_capital*physical_capital_price+firm_inventories+firm_endowment_payment_account;
	
/* 	printf("\n Government_initialization: hh_list.size = %d\n ", hh_list.size);
	printf("\n Government_initialization: number_of_firms = %d\n ", number_of_firms);
	printf("\n Government_initialization: total_production = %2.5f\n ", total_production);
	printf("\n Government_initialization: firm_production = %2.5f\n ", firm_production);
	printf("\n Government_initialization: firm_capital = %2.5f\n ", firm_capital);
	printf("\n Government_initialization: firm_inventories = %2.5f\n ", firm_inventories); */
	

	// firm payment_account, non_current_liabilities and current_liabilities are equal to ZERO!
	
	// initialize bank balance sheet
	
	double hh_endowment_payment_account = MEAN_WAGE;
	
	double bank_deposits = hh_list.size*hh_endowment_payment_account + firm_endowment_payment_account; // each household is endowed with 1 mean salary.
	double bank_equity = bank_deposits/2;
	double bank_payment_account = bank_deposits + bank_equity;
	double bank_cb_debt = 0.0;
	
	
	// asign shares to households
	///////////////////////////////////////////////////////////
	
		START_FIRM_TO_GOV_INI_MESSAGE_LOOP
			
			add_Enterprise_initialization(&firm_list,
			firm_to_gov_ini_message->firm_id,
			firm_to_gov_ini_message->outstanding_shares,
			firm_equity/firm_to_gov_ini_message->outstanding_shares);
		
		FINISH_FIRM_TO_GOV_INI_MESSAGE_LOOP
		
		START_PCFIRM_TO_GOV_INI_MESSAGE_LOOP
			
			add_Enterprise_initialization(&pcfirm_list,
			pcfirm_to_gov_ini_message->pcfirm_id,
			pcfirm_to_gov_ini_message->outstanding_shares, 0);
			
		
		FINISH_PCFIRM_TO_GOV_INI_MESSAGE_LOOP
		
		START_BANK_TO_GOV_INI_MESSAGE_LOOP
			
			add_Enterprise_initialization(&bank_list,
			bank_to_gov_ini_message->bank_id,
			bank_to_gov_ini_message->outstanding_shares,
			bank_equity/bank_to_gov_ini_message->outstanding_shares);
		
		FINISH_BANK_TO_GOV_INI_MESSAGE_LOOP
		

		int i = 0;
		int j = 0;
		while(firm_list.size > 0)
		{
			if(i >= firm_list.size)
			i = 0;
		
			if(j == hh_list.size)
			j = 0;
		
			add_gov_to_hh_init_shares_message(hh_list.array[j], firm_list.array[i].enterprise_id, 
			1, firm_list.array[i].share_book_value);
			
			firm_list.array[i].outstanding_shares--;
			
			if(firm_list.array[i].outstanding_shares == 0)
			{
				remove_Enterprise_initialization(&firm_list, i);
			}
			else
			{
				i++;
			}

			j++;
		}
		
		i = 0;
		while(pcfirm_list.size > 0)
		{
			if(i >= pcfirm_list.size)
			i = 0;
		
			if(j == hh_list.size)
			j = 0;
		
			add_gov_to_hh_init_shares_message(hh_list.array[j], pcfirm_list.array[i].enterprise_id, 
			1, pcfirm_list.array[i].share_book_value);
			
			pcfirm_list.array[i].outstanding_shares--;
			
			if(pcfirm_list.array[i].outstanding_shares == 0)
			{
				remove_Enterprise_initialization(&pcfirm_list, i);	
			}
			else
			{
				i++;
			}

			j++;
		}
		
		i = 0;
		while(bank_list.size > 0)
		{
			if(i >= bank_list.size)
			i = 0;
		
			if(j == hh_list.size)
			j = 0;
		
			add_gov_to_hh_init_shares_message(hh_list.array[j], bank_list.array[i].enterprise_id, 
			1, bank_list.array[i].share_book_value);
			
			bank_list.array[i].outstanding_shares--;
			
			if(bank_list.array[i].outstanding_shares == 0)
			{
				remove_Enterprise_initialization(&bank_list, i);
			}
			else
			{
				i++;
			}
				
			j++;
		}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	reset_Enterprise_initialization_array(&firm_list);
		
	START_FIRM_TO_GOV_INI_MESSAGE_LOOP
		
		add_Enterprise_initialization(&firm_list,
		firm_to_gov_ini_message->firm_id,
		firm_to_gov_ini_message->outstanding_shares,
		firm_equity/firm_to_gov_ini_message->outstanding_shares);
	
	FINISH_FIRM_TO_GOV_INI_MESSAGE_LOOP


	/*The government assigns a day of month to act for each firm*/
	int_array activation_days_list;
	init_int_array(&activation_days_list);
	
	int min_firms_per_day = 2;
	int days_in_week[]={1,3,5,2,4};
	int day = days_in_week[0];
	int week=1;
	
	i=1;
	int k=0;
	while(i <= firm_list.size)
	{
		if(day == 20)
		{
			add_int(&activation_days_list, 0);
		}
		else
		{
			add_int(&activation_days_list, day);
		}
		
		
		if(i%min_firms_per_day == 0)
		{
			if(week == 4)
			{
				k++;
				day = days_in_week[k%5];
				week = 1;
			}
			else
			{
				week++;
				day = day+5;
			}	
		}
		
		if(k == 5) min_firms_per_day = 1;
		i++;
	}
	
	
	// asign workers to companies
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
		
		j = 0;
		for(i = 0; i < hh_list.size; i++)
		{
			if(j == firm_list.size)
			j = 0;
			
			add_gov_init_labor_message(hh_list.array[i], firm_list.array[j].enterprise_id, activation_days_list.array[j], MEAN_WAGE);
			
			j++;			
		}	

	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	// initialize agents' balance sheets
	
	// gov_init_balance_sheets message structure:
	//<!-- (agent_id, current_assets, non_current_assets, payment_account, current_liabilities, non_current_liabilities) -->
	
	///////////////////////////////////////////////////////////////////////////////////////////////

		START_FIRM_TO_GOV_INI_MESSAGE_LOOP
		
			add_gov_init_balance_sheets_message(firm_to_gov_ini_message->firm_id,
			firm_inventories, firm_capital, firm_endowment_payment_account, 0, 0);
			
		FINISH_FIRM_TO_GOV_INI_MESSAGE_LOOP
		
		START_HH_TO_GOV_INI_MESSAGE_LOOP
	
			add_gov_init_balance_sheets_message(hh_to_gov_ini_message->hh_id,
			0, 0, hh_endowment_payment_account, 0, 0);
	
		FINISH_HH_TO_GOV_INI_MESSAGE_LOOP
		
		START_BANK_TO_GOV_INI_MESSAGE_LOOP
			
			add_gov_init_balance_sheets_message(bank_to_gov_ini_message->bank_id,
			0, 0, bank_payment_account, bank_deposits, 0);
			
		FINISH_BANK_TO_GOV_INI_MESSAGE_LOOP

	///////////////////////////////////////////////////////////////////////////////////////////////
	
	CB_DEBT = (MEAN_WAGE*hh_list.size*12)/2;
	
	PAYMENT_ACCOUNT = hh_list.size*MEAN_WAGE;
	
	CURRENT_ASSETS = 0;
	NON_CURRENT_ASSETS = 0;

	
	TOTAL_ASSETS = CURRENT_ASSETS + NON_CURRENT_ASSETS + PAYMENT_ACCOUNT;
	TOTAL_LIABILITIES = CB_DEBT;
	EQUITY = TOTAL_ASSETS - TOTAL_LIABILITIES;
	
	double cb_payment_account = bank_payment_account + PAYMENT_ACCOUNT;
	double fiat_money = CB_DEBT + bank_cb_debt;
	double cb_deposits = cb_payment_account;
	
	add_gov_init_balance_sheets_message(cb_id, bank_cb_debt, CB_DEBT, cb_payment_account, cb_deposits, fiat_money);
	
	
	
	free_int_array(&hh_list);
	free_Enterprise_initialization_array(&firm_list);
	free_Enterprise_initialization_array(&pcfirm_list);
	free_Enterprise_initialization_array(&bank_list);
	
	INITIALIZATION = 0;

    return 0;
}