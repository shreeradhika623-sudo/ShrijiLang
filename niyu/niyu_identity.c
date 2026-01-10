#include "niyu.h"
#include <stdio.h>
#include <string.h>

int positive_identity = 0;
int negative_identity = 0;
int mixed_identity = 0;
void niyu_identity(const char *msg) {
(void)msg;

    printf("[Identity🧬] Mapping personality pattern...\n");

    const char *emotion = niyu_emotion_last();

    if (strcmp(emotion, "positive") == 0) {
        positive_identity++;
        printf("[Identity📈] Growth identity strengthened.\n");
    }
    else if (strcmp(emotion, "negative") == 0) {
        negative_identity++;
        printf("[Identity📉] Fear / doubt identity increased.\n");
    }
    else {
        mixed_identity++;
        printf("[Identity⚪] Mixed identity signal detected.\n");
    }

    // Print summary
    printf("[Identity📊] Stats → Positive:%d  Negative:%d  Mixed:%d\n",
            positive_identity, negative_identity, mixed_identity);

    printf("[Identity✔️] Personality map updated.\n");
}
