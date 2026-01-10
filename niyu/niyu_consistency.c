#include "niyu.h"
#include <stdio.h>
#include <string.h>

// --------------------------------------
//  LAST MESSAGE MEMORY (GLOBAL VARIABLE)
// --------------------------------------
extern const char *niyu_last_message_global;  

// --------------------------------------------------
// Check if new message contradicts last remembered message
// --------------------------------------------------
int internal_contradiction(const char *msg, const char *last_msg) {

    if (!last_msg) return 0;

    // Case 1: User improved after failing
    if (strstr(msg, "improve") && strstr(last_msg, "failed"))
        return 1;

    // Case 2: User says cannot even though earlier they wanted something
    if (strstr(msg, "cannot") && strstr(last_msg, "want"))
        return 1;

    // Case 3: User expects success despite no action (expanded rule)
    if ((strstr(msg, "will succeed") || strstr(msg, "I will succeed")) &&
        strstr(last_msg, "do nothing"))
        return 1;

    return 0;
}

// --------------------------------------------------
// SELF-CONSISTENCY ENGINE
// --------------------------------------------------
void niyu_consistency(const char *msg) {

    printf("[Self🔁] Checking internal consistency...\n");

    const char *last = niyu_last_message_global;

    if (!last) {
        printf("[Self🔁] No previous message → Cannot compare.\n");
        return;
    }

    if (internal_contradiction(msg, last)) {
        printf("[Self⚠️] Inconsistency detected with previous statement.\n");
        printf("[Self⚠️] Your reasoning does not match your earlier message.\n");
        return;
    }

    printf("[Self✔️] Statement consistent with previous reasoning.\n");
}
