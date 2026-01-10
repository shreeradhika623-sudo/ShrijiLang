#include "niyu.h"
#include <stdio.h>
#include <string.h>

// Small helper to convert strstr result into clean 0/1 output
int contains(const char *msg, const char *word) {
    return strstr(msg, word) != NULL;
}

int has_want(const char *msg) {
    return contains(msg, "want");
}

int has_no_action(const char *msg) {
    return contains(msg, "do nothing") || contains(msg, "nothing");
}

int has_belief(const char *msg) {
    return contains(msg, "believe") || contains(msg, "think");
}

int has_improve(const char *msg) {
    return contains(msg, "improve") || contains(msg, "progress");
}

// ---------------------------------------------------------
// MAIN RESOLUTION LOGIC
// ---------------------------------------------------------
void niyu_resolve(const char *msg) {

    printf("[Resolve⚖️] Evaluating contradictions...\n");

    if (has_want(msg) && has_no_action(msg)) {
        printf("[Resolve⚖️] Contradiction: Desire exists but action is missing.\n");
        printf("[Resolve⚖️] Logical Result: Outcome cannot match desire.\n");
        return;
    }

    if (has_belief(msg) && !has_improve(msg)) {
        printf("[Resolve⚖️] Notice: Belief detected without supporting evidence.\n");
        printf("[Resolve⚖️] Logical Result: Belief alone cannot guarantee outcome.\n");
        return;
    }

    if (has_improve(msg) && contains(msg, "failed")) {
        printf("[Resolve⚖️] Mixed pattern: Failure + improvement.\n");
        printf("[Resolve⚖️] Logical Result: Improvement reduces effect of past failure.\n");
        return;
    }

    printf("[Resolve⚖️] No contradiction found.\n");
}
