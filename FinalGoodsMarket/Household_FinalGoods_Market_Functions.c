#include "../header.h"
#include "../my_library_header.h"

#include "../Household_agent_header.h"

/* \fn: int Household_plan_consumption_budget()
 
* \brief: Household_plan_consumption_budget.
 
 
* \timing: 
 * \condition:
 

* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_plan_consumption_budget()
{
	

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
	add_final_goods_order_message(ID, WEEKLY_BUDGET);

    return 0;
}

/* \fn: int Household_receive_final_goods()
 
* \brief: Household receive final goods.
 
 
* \timing: Weekly
 * \condition:
*\ filter message: a.id == m.buyer_id
*\ final_goods_delivery_to_buyers message structure:  <!-- (buyer_id, remaining_budget, costs, quantity) -->

* \authors: Marko Petrovic
* \history: 25.10.2017-Marko: First implementation.
*/
int Household_receive_final_goods()
{
	

    return 0;
}







