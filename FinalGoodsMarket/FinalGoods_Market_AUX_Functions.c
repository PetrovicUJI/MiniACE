#include "../header.h"
#include "FinalGoods_Market_AUX_Headers.h"

int final_goods_list_rank_price_function(const void *x, const void *y)
{
    if( ((Final_good *)x)->price > ((Final_good *)y)->price) return -1;
    else if( ((Final_good *)x)->price < ((Final_good *)y)->price) return 1;
    else
    {
        /* If equal randomly sort */
        if(rand()/((double)RAND_MAX + 1) <= 0.5) return -1;
        else return 1;
    }
}