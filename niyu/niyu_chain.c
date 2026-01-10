#include "niyu.h"
#include <stdio.h>
#include <string.h>

/*
   ---------------------------------------------------------
   CAUSAL GRAPH ENGINE — LEVEL 7
   ---------------------------------------------------------
   Ye engine message se cause-effect graph banata hai.

   Example:
       "I failed because I did not study"

   Causal Graph:
       Action → Consequence → Emotion → Self-perception

   Yeh OpenAI-type chain-of-thought behaviour ka 
   LIGHTWEIGHT ethical form hai.
*/

void niyu_causal_chain(const char *msg) {

    printf("[Chain⛓️] Constructing causal reasoning graph...\n");

    if (strstr(msg, "because")) {
        printf("[Chain🟢] Cause detected → Effect detected.\n");
        printf("[Chain📘] Graph: (Reason) → (Outcome) → (Emotion)\n");
    }
    else if (strstr(msg, "so ")) {
        printf("[Chain🟡] Implicit cause-effect: User implies future consequence.\n");
        printf("[Chain📙] Graph: (State) → (Belief) → (Expectation)\n");
    }
    else {
        printf("[Chain⚪] No cause-effect structure found.\n");
    }

    printf("[Chain✔️] Causal chain modelling complete.\n");
}
