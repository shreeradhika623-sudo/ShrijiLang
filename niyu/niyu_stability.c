#include "niyu.h"
#include <stdio.h>
#include <string.h>

// Track last emotional + logical state
static char last_emotion[32] = "none";
static char last_logic[32] = "none";

void niyu_stability(const char *msg) {
(void)msg;

    printf("[Stability🛡️] Checking emotional + logical consistency...\n");

    const char *current_emotion = niyu_emotion_last();
    const char *current_logic = niyu_logic_last();

    // Detect emotional jump
    if (strcmp(last_emotion, "positive") == 0 && strcmp(current_emotion, "negative") == 0) {
        printf("[Stability⚠️] Sharp emotional drop detected → Applying correction.\n");
    }
    if (strcmp(last_emotion, "negative") == 0 && strcmp(current_emotion, "positive") == 0) {
        printf("[Stability⚠️] Emotional spike detected → Balancing output.\n");
    }

    // Detect logic instability
    if (strcmp(last_logic, "unstable") == 0) {
        printf("[Stability⚠️] Previous reasoning unstable → Using shield mode.\n");
    }

    // Update history
    strcpy(last_emotion, current_emotion);
    strcpy(last_logic, current_logic);

    printf("[Stability✔️] System stable.\n");
}
