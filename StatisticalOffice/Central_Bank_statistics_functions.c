#include "../header.h"
#include "../my_library_header.h"

#include "../Central_Bank_agent_header.h"


/* \fn: int Central_Bank_info()
 
* \brief: Central_Bank info.
 
 
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Central_Bank_send_info()
{


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

		FINISH_STAT_OFFICE_SEND_STATISTICS_MESSAGE_LOOP

    return 0;
}