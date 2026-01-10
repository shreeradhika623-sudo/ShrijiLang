#include "niyu.h"
#include <stdio.h>
#include <string.h>

// Detect ambiguous messages
int is_ambiguous(const char *msg) {
    if (strstr(msg, "maybe")) return 1;
    if (strstr(msg, "not sure")) return 1;
    if (strstr(msg, "kind of")) return 1;
    return 0;
}

void niyu_clarity(const char *msg) {

    printf("[Clarity❓] Checking clarity...\n");

    if (!is_ambiguous(msg)) {
        printf("[Clarity❓] Message is clear.\n");
        return;
    }

    printf("[Clarity❓] Ambiguity detected.\n");
    printf("[Clarity❓] Would you like to clarify?\n");
}
