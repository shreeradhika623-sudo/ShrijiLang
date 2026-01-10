#include "niyu.h"
#include <stdio.h>
#include <string.h>

void niyu_anchor(const char *msg) {

    printf("[Anchor🧭] Detecting identity direction...\n");

    if (strstr(msg, "succeed") || strstr(msg, "improving")) {
        printf("[Anchor🌱] Identity → Growth mode.\n");
    }
    else if (strstr(msg, "failed") || strstr(msg, "nothing")) {
        printf("[Anchor🌧️] Identity → Fear / Doubt mode.\n");
    }
    else {
        printf("[Anchor⚪] Identity → Neutral / Undefined.\n");
    }

    printf("[Anchor✔️] Anchor check complete.\n");
}
