#include "niyu.h"
#include <string.h>
#include <stdio.h>

static int meta_score = 0;      // Tracks improvement
static int contradiction_hits = 0;
static int negative_hits = 0;
static int positive_hits = 0;

// Learn from contradictions
void meta_learn_contradiction(const char *msg) {
(void)msg;
    contradiction_hits++;
    printf("[Meta📊] Learning: Contradiction count = %d\n", contradiction_hits);

    if (contradiction_hits > 3) {
        printf("[Meta📈] Insight: User struggles with internal conflict.\n");
        meta_score++;
    }
}

// Learn from negativity
void meta_learn_negative(const char *msg) {
(void)msg; 
   negative_hits++;
    printf("[Meta📉] Learning: Negative count = %d\n", negative_hits);

    if (negative_hits > 5) {
        printf("[Meta🧠] Insight: User often loops in negative thoughts.\n");
        meta_score++;
    }
}

// Learn from positive improvements
void meta_learn_positive(const char *msg) {
(void)msg;  
  positive_hits++;
    printf("[Meta📈] Learning: Positive count = %d\n", positive_hits);

    if (positive_hits > 5) {
        printf("[Meta✨] Insight: User improvement trend detected.\n");
        meta_score++;
    }
}

// Final synthesis of meta-learning
void niyu_meta_learn(const char *msg) {

    printf("[Meta🧬] Meta-Learning active...\n");

    if (strstr(msg, "but"))
        meta_learn_contradiction(msg);

    if (strstr(msg, "fail") || strstr(msg, "can't") || strstr(msg, "stuck"))
        meta_learn_negative(msg);

    if (strstr(msg, "improve") || strstr(msg, "progress"))
        meta_learn_positive(msg);

    printf("[Meta🧪] Current Meta-Score: %d\n", meta_score);

    if (meta_score >= 5) {
        printf("[Meta🌟] Upgrade: Niyu’s internal reasoning improved.\n");
    }
}
