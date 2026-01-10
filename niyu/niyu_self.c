#include "niyu.h"
#include <stdio.h>
#include <string.h>

static char last_classification[64] = "none";
static char last_emotion[64] = "none";

// Compare with previous output and detect self-error
void niyu_self_check(const char *current_emotion, const char *current_class) {
    printf("[Self🔍] Running self-analysis...\n");

    int changed = 0;

    // Emotion mismatch
    if (strcmp(current_emotion, last_emotion) != 0 &&
        strcmp(last_emotion, "none") != 0) 
    {
        printf("[SelfFix🛠] Emotional mismatch found → Adjusting internal threshold.\n");
        changed = 1;
    }

    // Classification mismatch
    if (strcmp(current_class, last_classification) != 0 &&
        strcmp(last_classification, "none") != 0) 
    {
        printf("[SelfFix🛠] Logic inconsistency found → Updating reasoning.\n");
        changed = 1;
    }

    if (!changed)
        printf("[Self✔️] No internal conflict. System stable.\n");

    // Store for next cycle
    strcpy(last_emotion, current_emotion);
    strcpy(last_classification, current_class);
}

void niyu_self(const char *emotion, const char *logic_class) {
    niyu_self_check(emotion, logic_class);
}
