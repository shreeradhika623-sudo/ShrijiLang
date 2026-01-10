#include "niyu.h"
#include <stdio.h>

void niyu_why_engine(const char *emotion_label, const char *logic_label) {

    printf("[Why🔍] Explaining my reasoning...\n");

    niyu_reason_emotion(emotion_label);
    niyu_reason_logic(logic_label);

    printf("[Why✔️] Reasoning complete.\n");
}
