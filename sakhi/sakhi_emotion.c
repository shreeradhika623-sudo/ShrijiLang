#include <string.h>
#include <stdio.h>
#include "sakhi.h"

// -----------------------------------------
// ORIGINAL DETECTOR
// -----------------------------------------
SakhiEmotion sakhi_emotion_detect(const char *msg) {

    if (strstr(msg, "hurt") || strstr(msg, "toot"))
        return EMO_HURT;

    if (strstr(msg, "sad") || strstr(msg, "dukhi"))
        return EMO_SAD;

    if (strstr(msg, "alone") || strstr(msg, "lonely"))
        return EMO_LONELY;

    if (strstr(msg, "stress") || strstr(msg, "pressure"))
        return EMO_STRESSED;

    if (strstr(msg, "fear") || strstr(msg, "dar"))
        return EMO_FEAR;

    if (strstr(msg, "guilt") || strstr(msg, "galti"))
        return EMO_GUILT;

    if (strstr(msg, "happy") || strstr(msg, "khush"))
        return EMO_HAPPY;

    if (strstr(msg, "thank") || strstr(msg, "grateful"))
        return EMO_GRATEFUL;

    if (strstr(msg, "wow") || strstr(msg, "excited"))
        return EMO_EXCITED;

    return EMO_NEUTRAL;
}

// -----------------------------------------
// REQUIRED BY sakhi_bridge()
// -----------------------------------------
SakhiEmotion sakhi_emotion_process(const char *msg) {
    return sakhi_emotion_detect(msg);
}
