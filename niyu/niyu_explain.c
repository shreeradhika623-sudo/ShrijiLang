#include "niyu.h"
#include <stdio.h>
#include <string.h>

void niyu_reason_emotion(const char *emotion_label) {

    printf("[Why-Emotion💖] ");

    if (strcmp(emotion_label, "positive") == 0) {
        printf("Emotion was marked positive because the wording shows improvement or hope.\n");
    }
    else if (strcmp(emotion_label, "negative") == 0) {
        printf("Emotion was marked negative because the message contains pain, doubt, or failure.\n");
    }
    else {
        printf("Emotion was marked neutral because no emotional weight was detected.\n");
    }
}
