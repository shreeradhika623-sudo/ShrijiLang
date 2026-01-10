#include "niyu.h"
#include <stdio.h>
#include <string.h>

// MAKE THEM GLOBAL (NOT STATIC)
int count_negative = 0;
int count_positive = 0;


// -------------------------------------------
// BETTER NEGATIVE CHECK (FIXED)
// -------------------------------------------
int is_negative_event(const char *msg) {

    if (strstr(msg, "cannot")) return 1;
    if (strstr(msg, "can't")) return 1;
    if (strstr(msg, "never")) return 1;
    if (strstr(msg, "not improving")) return 1;
    if (strstr(msg, "no progress")) return 1;

    if (strstr(msg, "fail")) return 1;
    if (strstr(msg, "stuck")) return 1;
    if (strstr(msg, "nothing")) return 1;

    return 0;
}


// -------------------------------------------
// POSITIVE CHECK (CLEAN LOGIC)
// -------------------------------------------
int is_positive_event(const char *msg) {

    if (strstr(msg, "cannot improve")) return 0;
    if (strstr(msg, "can't improve")) return 0;

    if (strstr(msg, "improving")) return 1;
    if (strstr(msg, "better")) return 1;
    if (strstr(msg, "progress")) return 1;
    if (strstr(msg, "improve")) return 1;

    return 0;
}


// -------------------------------------------
// MAIN INDUCTIVE ENGINE
// -------------------------------------------
void niyu_induce(const char *msg) {

    printf("[Induce🔮] Inductive reasoning active...\n");

    int neg_flag = is_negative_event(msg);
    int pos_flag = is_positive_event(msg);

    // NEGATIVE FIRST
    if (neg_flag) {
        count_negative++;
        printf("[Induce🔮] Negative pattern +1 (Total: %d)\n", count_negative);
    }

    // POSITIVE AFTER (ONLY IF NOT NEGATIVE)
    if (pos_flag && !neg_flag) {
        count_positive++;
        printf("[Induce🔮] Positive pattern +1 (Total: %d)\n", count_positive);
    }

    if (count_negative >= 2)
        printf("[Induce🔮] Pattern: Repeated negative pattern detected.\n");

    if (count_positive >= 2)
        printf("[Induce🔮] Pattern: Positive improvement trend.\n");

    if (neg_flag && pos_flag)
        printf("[Induce🔮] Mixed pattern: User fluctuating.\n");

    if (!neg_flag && !pos_flag)
        printf("[Induce🔮] No pattern detected.\n");
}
