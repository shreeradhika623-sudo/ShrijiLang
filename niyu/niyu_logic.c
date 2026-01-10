#include "niyu.h"
#include <stdio.h>
#include <string.h>

// Global variable to store last logic classification
static char last_logic_label[64] = "none";

// Getter for Level-5 usage (bridge will use this)
const char* niyu_logic_last() {
    return last_logic_label;
}

void niyu_logic_process(const char *msg) {

    char logic_type[64] = "neutral";

    printf("[Logic🧠] Processing...\n");

    // -----------------------------------
    // BASIC LOGIC CLASSIFICATION MODEL
    // -----------------------------------
    if (strstr(msg, "cannot") || strstr(msg, "fail") || strstr(msg, "not"))
        strcpy(logic_type, "negative");

    else if (strstr(msg, "improve") || strstr(msg, "grow") || strstr(msg, "succeed"))
        strcpy(logic_type, "positive");

    else
        strcpy(logic_type, "neutral");

    printf("[Logic🧠] Analysis complete.\n");

    // Save for Level-5 usage
    strcpy(last_logic_label, logic_type);
}
