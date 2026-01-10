#include "niyu.h"
#include <stdio.h>
#include <string.h>

// A → B pattern detect
int has_implication(const char *msg) {
    return strstr(msg, "so") || strstr(msg, "therefore") || strstr(msg, "because");
}

// Check if message contains a clear cause → effect pattern
void niyu_deduce(const char *msg) {

    printf("[Deduce🔍] Running deductive reasoning...\n");

    if (has_implication(msg)) {
        printf("[Deduce🔍] Implication detected → User is drawing a logical conclusion.\n");

        // Basic rule: If message contains contradiction inside implication
        if (strstr(msg, "but")) {
            printf("[Deduce🔍] Warning: Contradiction inside deduction.\n");
        }

        printf("[Deduce🔍] Deduction processed.\n");
    }
    else {
        printf("[Deduce🔍] No deductive structure found.\n");
    }
}
