#include "niyu.h"
#include <stdio.h>
#include <string.h>

// External or shared counters
extern int count_negative;
extern int count_positive;

void niyu_synthesis(const char *msg) {
(void)msg;
    printf("[Synth🧬] Synthesizing understanding...\n");

    int neg = count_negative;
    int pos = count_positive;

    // If nothing detected
    if (neg == 0 && pos == 0) {
        printf("[Synth🧬] Stable message.\n");
        printf("[Synth🧬] Interpretation: No emotional or logical conflict detected.\n");
        return;
    }

    // PURE NEGATIVE
    if (neg > 0 && pos == 0) {
        printf("[Synth🧬] Negative emotional indication detected.\n");
        printf("[Synth🧬] Interpretation: User is feeling discouraged or stuck.\n");
        return;
    }

    // PURE POSITIVE
    if (pos > 0 && neg == 0) {
        printf("[Synth🧬] Positive growth signal detected.\n");
        printf("[Synth🧬] Interpretation: User is genuinely improving.\n");
        return;
    }

    // MIXED SIGNAL
    if (pos > 0 && neg > 0) {
        printf("[Synth🧬] Mixed emotional pattern detected.\n");
        printf("[Synth🧬] Interpretation: User is fluctuating between hope and doubt.\n");
        return;
    }

    // Default fallback
    printf("[Synth🧬] Stable message.\n");
    printf("[Synth🧬] Interpretation: No conflict detected.\n");
}
