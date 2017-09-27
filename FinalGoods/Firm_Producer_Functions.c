#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"




/* \fn: int Firm_plan_production()
 
* \brief: Firm determine production plan.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 27.09.2017-Marko: First implementation.
*/
int Firm_plan_production()
{
	EXPECTED_DEMAND = 0;
	
	for(int i = 0; i < SOLD_QUANTITIES_VECTOR.size; i++)
	{
		EXPECTED_DEMAND += SOLD_QUANTITIES_VECTOR.array[i];
	}
	
	EXPECTED_DEMAND = EXPECTED_DEMAND/SOLD_QUANTITIES_VECTOR.size;
	
	if(EXPECTED_DEMAND < CURRENT_ASSETS) {PRODUCTION_PLAN = 0;}
	else {PRODUCTION_PLAN = EXPECTED_DEMAND - CURRENT_ASSETS;}

    return 0;
}

int Firm_compute_sale_statistics()
{
	
	remove_double(&SOLD_QUANTITIES_VECTOR,0);
    add_double(&SOLD_QUANTITIES_VECTOR,MONTHLY_SOLD_QUANTITY);
	
	MONTHLY_SOLD_QUANTITY = 0;

    return 0;
}

