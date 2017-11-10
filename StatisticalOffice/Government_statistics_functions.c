#include "../header.h"
#include "../my_library_header.h"

#include "../Government_agent_header.h"


/* \fn: int Government_info()
 
* \brief: Government info.
 
 
 
* \authors: Marko Petrovic
* \history: 08.11.2017-Marko: First implementation.
*/
int Government_info()
{


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

    return 0;
}

