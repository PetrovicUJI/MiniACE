#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"




/* \fn: int Firm_plan_production_quantity()
 
* \brief: Firm determine production plan.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 27.09.2017-Marko: First implementation.
*/
int Firm_plan_production_quantity()
{
	double expected_demand = 0.0;
	
	for(int i = 0; i < SOLD_QUANTITIES_VECTOR.size; i++)
	{
		expected_demand += SOLD_QUANTITIES_VECTOR.array[i];
	}
	
	expected_demand = expected_demand/SOLD_QUANTITIES_VECTOR.size;
	
	if(expected_demand < CURRENT_ASSETS) {PRODUCTION_PLAN = 0;}
	else {PRODUCTION_PLAN = expected_demand - CURRENT_ASSETS;}

    return 0;
}


/* \fn: int Firm_plan_labor_demand()
 
* \brief: Firm determine labor demand.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 04.10.2017-Marko: First implementation.
*/
int Firm_plan_labor_demand()
{

	CAPACITY_UTILIZATION = min(1,(PRODUCTION_PLAN/(PHYSICAL_CAPITAL*PRODUCTIVITY)));
	
	LABOR_REQUIREMENT = CAPACITY_UTILIZATION*(PHYSICAL_CAPITAL/CAPITAL_LABOR_RATIO);

    return 0;
}

/* \fn: int Firm_compute_sale_statistics()
 
* \brief: Firm store data about a monthly sold quantity.
 
 
* \timing: Monthly on the last activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 27.09.2017-Marko: First implementation.
*/
int Firm_compute_sale_statistics()
{
	
	remove_double(&SOLD_QUANTITIES_VECTOR,0);
    add_double(&SOLD_QUANTITIES_VECTOR,MONTHLY_SOLD_QUANTITY);
	
	MONTHLY_SOLD_QUANTITY = 0;

    return 0;
}

