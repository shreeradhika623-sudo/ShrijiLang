#include "niyu.h"
#include <stdio.h>
#include <string.h>

void niyu_reason_logic(const char *logic_label) {

    printf("[Why-Logic🧠] ");

    if (strcmp(logic_label, "positive") == 0) {
        printf("I marked it positive because the message structure shows confidence and growth.\n");
    }
    else if (strcmp(logic_label, "negative") == 0) {
        printf("I marked it negative because the sentence contains self-doubt or failure cues.\n");
    }
    else {
        printf("I marked it neutral because the message has no strong logical signals.\n");
    }
}
