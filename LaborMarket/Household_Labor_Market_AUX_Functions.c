#include "../header.h"
#include "Household_Labor_Market_AUX_Headers.h"

int job_offer_list_rank_wage_offer_function(const void *x, const void *y)
{
    if( ((Job_offer *)x)->wage > ((Job_offer *)y)->wage) return -1;
    else if( ((Job_offer *)x)->wage < ((Job_offer *)y)->wage) return 1;
    else
    {
        /* If equal randomly sort */
        if(rand()/((double)RAND_MAX + 1) <= 0.5) return -1;
        else return 1;
    }
}