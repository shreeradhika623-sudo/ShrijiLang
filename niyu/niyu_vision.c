#include "niyu.h"
#include <stdio.h>
#include <string.h>

static int positive_count = 0;
static int negative_count = 0;

// Detect long-term user goal
void niyu_vision(const char *msg) {
    printf("[Vision👁️] Scanning long-term intent...\n");

    if (strstr(msg, "improve") || strstr(msg, "grow") || strstr(msg, "succeed"))
        positive_count++;

    if (strstr(msg, "fail") || strstr(msg, "stuck") || strstr(msg, "nothing"))
        negative_count++;

    if (positive_count >= 2) {
        printf("[Vision👁️] Long-term goal recognized → User aims for improvement & success.\n");
    }

    if (negative_count >= 2) {
        printf("[Vision👁️] Warning → User facing repeated setbacks.\n");
    }

    if (positive_count == 0 && negative_count == 0)
        printf("[Vision👁️] No long-term pattern detected yet.\n");
}
