#include "../header.h"
#include "../my_library_header.h"

#include "../Central_Bank_agent_header.h"




/* \fn: int Central_Bank_info()
 
* \brief: Central_Bank info.
 
*\ cb_send_info_message structure: 
<!-- (price_level, inflation, cb_interest_rate, alpha_capital_requirement, inflation_target) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Central_Bank_send_info()
{
	add_cb_send_info_message(PRICE_LEVEL, INFLATION, CB_INTEREST_RATE, ALPHA_CAPITAL_REQUIREMENT, INFLATION_TARGET);

    return 0;
}


/* \fn: int CB_receive_statistics()
 
* \brief: CB receive statistics.
 
 stat_office_send_statistics_message structure: <!-- (monthly_gdp, price_level, inflation, value_at_risk, mean_wage, unemployment_rate) -->
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int CB_receive_statistics()
{
		START_STAT_OFFICE_SEND_STATISTICS_MESSAGE_LOOP
		
			INFLATION = stat_office_send_statistics_message->inflation;
			VALUE_AT_RISK = stat_office_send_statistics_message->value_at_risk;
			UNEMPLOYMENT_RATE = stat_office_send_statistics_message->unemployment_rate;
			PRICE_LEVEL = stat_office_send_statistics_message->price_level;

		FINISH_STAT_OFFICE_SEND_STATISTICS_MESSAGE_LOOP

    return 0;
}