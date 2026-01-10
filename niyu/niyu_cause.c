#include "niyu.h"
#include <stdio.h>
#include <string.h>

// Check if message contains a cause phrase
int contains_cause(const char *msg) {
    if (strstr(msg, "because")) return 1;
    if (strstr(msg, "since")) return 1;
    if (strstr(msg, "due to")) return 1;
    return 0;
}

// Extract reason part (simple version)
void extract_cause(const char *msg) {

    printf("[Cause🔍] Scanning reasoning...\n");

    if (!contains_cause(msg)) {
        printf("[Cause🔍] No cause detected.\n");
        return;
    }

    const char *ptr = NULL;

    if ((ptr = strstr(msg, "because")))
        ptr += strlen("because");

    else if ((ptr = strstr(msg, "since")))
        ptr += strlen("since");

    else if ((ptr = strstr(msg, "due to")))
        ptr += strlen("due to");

    if (ptr) {
        printf("[Cause🔍] Detected reason: %s\n", ptr);
        printf("[Cause🔍] Interpretation: User provided a cause or justification.\n");
    }
}

// MAIN interface
void niyu_cause(const char *msg) {
    extract_cause(msg);
}
