#include "../header.h"
#include "../my_library_header.h"

#include "../Firm_agent_header.h"






/* \fn: int Firm_update_capacity()
 
* \brief: Firm update capital stock.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 05.10.2017-Marko: First implementation.
*/
int Firm_update_capacity()
{
	int size = PHYSICAL_CAPITAL_STOCK.size;
	int i = 0;
	PHYSICAL_CAPITAL = 0;
	
	while(i < size)
	{
		//update after the production takes place and before new capital is bouth
		//PHYSICAL_CAPITAL_STOCK.array[i].months_in_use++;

		if(PHYSICAL_CAPITAL_STOCK.array[i].months_in_use >= PHYSICAL_CAPITAL_DURATION)
		{
			remove_physical_capital_batch(&PHYSICAL_CAPITAL_STOCK,i);
			
			size--;
		}
		else
		{
			PHYSICAL_CAPITAL_STOCK.array[i].current_value = PHYSICAL_CAPITAL_STOCK.array[i].capital_units*PHYSICAL_CAPITAL_STOCK.array[i].price*(1-(PHYSICAL_CAPITAL_STOCK.array[i].months_in_use/PHYSICAL_CAPITAL_DURATION));
			
			PHYSICAL_CAPITAL += PHYSICAL_CAPITAL_STOCK.array[i].capital_units;
			
			i++;
		}
	}

    return 0;
}




/* \fn: int Firm_plan_production_quantity()
 
* \brief: Firm determine production plan.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 27.09.2017-Marko: First implementation.
*/
int Firm_plan_production_quantity()
{
	EXPECTED_DEMAND = 0.0;
	
	for(int i = 0; i < SOLD_QUANTITIES_VECTOR.size; i++)
	{
		EXPECTED_DEMAND += SOLD_QUANTITIES_VECTOR.array[i];
	}
	
	EXPECTED_DEMAND = EXPECTED_DEMAND/SOLD_QUANTITIES_VECTOR.size;
	
	if(EXPECTED_DEMAND < INVENTORIES) {PRODUCTION_PLAN = 0;}
	else {PRODUCTION_PLAN = EXPECTED_DEMAND - INVENTORIES;}

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


/* \fn: int Firm_plan_investment()
 
* \brief: Firm plan investment in physical capital.
 
 
* \timing: Monthly on the firm activation day.
 * \condition:
 
* \authors: Marko Petrovic
* \history: 05.10.2017-Marko: First implementation.
*/
int Firm_plan_investment()
{
	double profit_rate = 0.0;
	double capacity_growth_rate = 0.0;
	
	INVESTMENT_PLAN = 0;
	
	profit_rate = OPERATING_CASH_FLOW/NON_CURRENT_ASSETS;
	
	capacity_growth_rate = PROFIT_RATE_WEIGHTS*((profit_rate-TARGET_PROFIT_RATE)/TARGET_PROFIT_RATE) + CAPACITY_UTILIZATION_WEIGHTS*((CAPACITY_UTILIZATION-TARGET_CAPACITY_UTILIZATION)/TARGET_CAPACITY_UTILIZATION);

	if(capacity_growth_rate > 0) {INVESTMENT_PLAN = capacity_growth_rate*PHYSICAL_CAPITAL;}

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

