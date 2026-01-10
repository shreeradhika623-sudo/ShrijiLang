#include "niyu.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// -----------------------------------------
// Detect hesitation patterns
// -----------------------------------------
int detect_hesitation(const char *msg) {
    return (
        strstr(msg, "maybe") ||
        strstr(msg, "not sure") ||
        strstr(msg, "I feel")
    ) ? 1 : 0;
}

// -----------------------------------------
// Detect negative emotional tone
// -----------------------------------------
int detect_negative(const char *msg) {
    return (
        strstr(msg, "stuck") ||
        strstr(msg, "can't") ||
        strstr(msg, "nothing")
    ) ? 1 : 0;
}

// -----------------------------------------
// Detect logical contradiction marker
// -----------------------------------------
int detect_contradiction(const char *msg) {
    return (strstr(msg, "but")) ? 1 : 0;
}

// -----------------------------------------
// Main Observation Layer
// -----------------------------------------
void niyu_observe(const char *msg) {

    printf("[Obs👁️] Observing...\n");

    if (detect_hesitation(msg))
        printf("[Obs👁️] Pattern: Hesitation detected.\n");

    if (detect_negative(msg))
        printf("[Obs👁️] Emotional shift: Negative tone detected.\n");

    if (detect_contradiction(msg))
        printf("[Obs👁️] Logic Notice: Possible contradiction in statement.\n");

    // Default intent scanning
    if (strstr(msg, "want"))
        printf("[Obs👁️] Intent: User wants improvement or change.\n");

    // If nothing detected
    if (!detect_hesitation(msg) &&
        !detect_negative(msg) &&
        !detect_contradiction(msg))
    {
        printf("[Obs👁️] Observation: Message stable and logical.\n");
    }
}
