#include "../header.h"
#include "../my_library_header.h"

#include "../Government_agent_header.h"


/* \fn: int Government_send_info()
 
* \brief: Government send info.

* \timing: Monthly, the first day.
 
 *\ gov_send_info_message structure: 
 <!-- (mean_wage, vat_rate, income_tax_rate, capital_tax_rate, 
		unemployment_benefits_rate, public_transfers_rate) -->
 
* \authors: Marko Petrovic
* \history: 13.11.2017-Marko: First implementation.
*/
int Government_send_info()
{

	add_gov_send_info_message(MEAN_WAGE, VAT_RATE, INCOME_TAX_RATE, CAPITAL_TAX_RATE,
	UNEMPLOYMENT_BENEFITS_RATE, PUBLIC_TRANSFERS_RATE);
	
	MONTHLY_BENEFITS_EXPENDITURE = 0.0;
	MONTHLY_PUBLIC_TRANSFERS_EXPENDITURE = 0.0;
	MONTHLY_BAILOUT_EXPENDITURE = 0.0;
	MONTHLY_TAX_INCOME = 0.0;
	MONTHLY_INTEREST_PAYMENTS = 0.0;

    return 0;
}

/* \fn: int Government_receive_statistics()
 
* \brief: Government receive statistics.
 
 stat_office_send_statistics_message structure: <!-- (monthly_gdp, price_level, inflation, value_at_risk, mean_wage, unemployment_rate) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Government_receive_statistics()
{
		START_STAT_OFFICE_SEND_STATISTICS_MESSAGE_LOOP
		
			MONTHLY_GDP = stat_office_send_statistics_message->monthly_gdp;
			PRICE_LEVEL = stat_office_send_statistics_message->price_level;
			INFLATION = stat_office_send_statistics_message->inflation;
			MEAN_WAGE = stat_office_send_statistics_message->mean_wage;
			UNEMPLOYMENT_RATE = stat_office_send_statistics_message->unemployment_rate;

		FINISH_STAT_OFFICE_SEND_STATISTICS_MESSAGE_LOOP
		
		YEARLY_GDP += MONTHLY_GDP;

    return 0;
}

