#include "niyu.h"
#include <stdio.h>
#include <string.h>

// Check for outcome patterns
int contains_effect(const char *msg) {
    if (strstr(msg, "so")) return 1;
    if (strstr(msg, "therefore")) return 1;
    if (strstr(msg, "will happen")) return 1;
    if (strstr(msg, "will succeed")) return 1;
    return 0;
}

void extract_effect(const char *msg) {

    printf("[Effect🔮] Predicting outcome...\n");

    if (!contains_effect(msg)) {
        printf("[Effect🔮] No outcome pattern found.\n");
        return;
    }

    if (strstr(msg, "will succeed")) {
        printf("[Effect🔮] Prediction: User expects future success.\n");
        return;
    }

    const char *ptr = strstr(msg, "so");
    if (ptr) {
        printf("[Effect🔮] Detected result: %s\n", ptr + 2);
        return;
    }

    printf("[Effect🔮] Outcome detected but could not parse fully.\n");
}

void niyu_effect(const char *msg) {
    extract_effect(msg);
}
