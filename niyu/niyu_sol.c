#include <stdio.h>
#include <string.h>
#include "../include/niyu.h"

// --------------------------------------------------------
// SOL — SELF OPTIMIZE LAYER (Level 12 - Module 2)
// --------------------------------------------------------
// Ye engine SELF-HEAL ke baad deep systems ko adjust karta hai.
// Negative identity, mixed signals, instability ko cleanup karta hai.
// --------------------------------------------------------

void niyu_self_optimize(const char *msg) {
(void)msg;

    printf("[SOL⚙️] Self-Optimize Layer Activated...\n");

    // --------------------------------------------
    // 1. If emotion and logic mismatch — Fix Identity
    // --------------------------------------------
    if (strcmp(niyu_emotion_last(), "negative") == 0 &&
        strcmp(niyu_logic_last(), "neutral") == 0)
    {
        printf("[SOL⚙️] Aligning identity map...\n");
        negative_identity--;
        mixed_identity--;
        positive_identity++;

        if (negative_identity < 0) negative_identity = 0;
        if (mixed_identity   < 0) mixed_identity   = 0;
    }

    // --------------------------------------------
    // 2. Improve Emotional Stability Index
    // --------------------------------------------
    printf("[SOL⚙️] Stabilizing Emotional Index...\n");
    global_esi += 2;
    if (global_esi > 10) global_esi = 10;

    // --------------------------------------------
    // 3. Predict Engine Correction
    // --------------------------------------------
    printf("[SOL⚙️] Correcting prediction path...\n");

    // If negative identity > positive, push back toward stable state
    if (negative_identity > positive_identity) {
        positive_identity++;
        negative_identity--;
    }

    // --------------------------------------------
    // 4. Cognitive Cleanup
    // --------------------------------------------
    printf("[SOL⚙️] Cleaning cognitive noise...\n");
    // (Future expansion: remove contradictory patterns)

    // --------------------------------------------
    // 5. Final Confirmation
    // --------------------------------------------
    printf("[SOL⚙️] Optimization Complete.\n");
}
