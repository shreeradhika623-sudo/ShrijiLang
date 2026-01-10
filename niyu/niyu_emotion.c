#include "niyu.h"
#include <stdio.h>
#include <string.h>

static char last_emotion_label[32] = "neutral";
int emotion_score = 0;

void niyu_emotion(const char *msg) {

    printf("[Emotion🔍] Analyzing emotional tone...\n");

    // DEFAULT
    strcpy(last_emotion_label, "neutral");
    emotion_score = 0;

    // POSITIVE markers
    if (strstr(msg, "happy") || strstr(msg, "strong") || strstr(msg, "love") ||
        strstr(msg, "great") || strstr(msg, "good") || strstr(msg, "excited")) {

        emotion_score = 1;
        strcpy(last_emotion_label, "positive");

        printf("[Emotion💖] Positive emotion detected.\n");
        return;
    }

    // NEGATIVE markers
    if (strstr(msg, "sad") || strstr(msg, "scared") || strstr(msg, "hate") ||
        strstr(msg, "weak") || strstr(msg, "bad") || strstr(msg, "hopeless")) {

        emotion_score = -1;
        strcpy(last_emotion_label, "negative");

        printf("[Emotion💔] Negative emotion detected.\n");
        return;
    }

    // NEUTRAL remains default
    printf("[Emotion⚪] Neutral emotion.\n");
}

const char* niyu_emotion_last() {
    return last_emotion_label;
}
