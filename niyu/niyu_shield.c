#include "niyu.h"
#include <stdio.h>
#include <string.h>

void niyu_shield(const char *msg) {

    printf("[Shield🛡️] Monitoring for reasoning overload...\n");

    const char *emotion = niyu_emotion_last();

    if (strcmp(emotion, "overload") == 0) {
        printf("[Shield🚨] Emotional overload → Blocking extreme interpretations.\n");
        return;
    }

    if (strstr(msg, "never") || strstr(msg, "impossible")) {
        printf("[Shield🚫] Extreme-negative language blocked.\n");
        return;
    }

    if (strstr(msg, "always") || strstr(msg, "perfect")) {
        printf("[Shield🚫] Unrealistic-positive language blocked.\n");
        return;
    }

    printf("[Shield✔️] No overload. Reasoning safe.\n");
}
