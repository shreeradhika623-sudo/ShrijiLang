#include "../include/niyu.h"
#include <stdio.h>

// GLOBAL ESI SCORE
int global_esi = 0;

// -----------------------------------------
//  LEVEL-10: Emotional Stability Index
// -----------------------------------------
void niyu_update_esi() {

    // Formula:
    //  (positive ×2) – (negative ×2) + mixed
    global_esi = (positive_identity * 2)
               - (negative_identity * 2)
               + (mixed_identity);

    printf("[Meta10⚙️] Updating Emotional Stability Index...\n");
    printf("[Meta10📊] ESI = %d\n", global_esi);

    if (global_esi > 3)
        printf("[Meta10🟢] Emotional Trend: Strong Stability.\n");
    else if (global_esi >= 0)
        printf("[Meta10⚪] Emotional Trend: Neutral Stability.\n");
    else
        printf("[Meta10🔻] Emotional Trend: Risk — Emotional Decline.\n");
}

// getter
int niyu_get_esi() {
    return global_esi;
}
