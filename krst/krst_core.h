#ifndef KRST_CORE_H
#define KRST_CORE_H

#include "../include/pragya_avastha.h"

/*
    KRST Core Interface

    Role:
    - Main processing engine
    - Takes PragyaAvastha as input
    - Updates it (error, correction, teaching, etc.)
*/

/* Main processing function */
int krst_process(PragyaAvastha *avastha);

#endif
