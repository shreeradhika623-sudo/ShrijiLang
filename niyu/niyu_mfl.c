#include <stdio.h>
#include <string.h>
#include "../include/niyu.h"

//
// MEMORY FUSION LAYER (MFL)
// -----------------------------------------------
// Purpose:
// 1. Past memories analyze
// 2. Emotional patterns merge
// 3. Negative-weight reduce
// 4. Positive anchors boost
// 5. Final stability summary produce
//

void niyu_memory_fusion() {

    printf("[MFL🧠] Memory Fusion Layer Active...\n");

    int total = niyu_memory_count();
    if (total == 0) {
        printf("[MFL🧠] No memories to fuse.\n");
        return;
    }

    int positive = 0;
    int negative = 0;
    int mixed = 0;

    // Analyze all 251 memory entries
    for (int i = 0; i < total; i++) {

        const char *msg = niyu_memory_get(i);
        if (!msg) continue;

        if (strstr(msg, "happy") || strstr(msg, "win") || strstr(msg, "good"))
            positive++;

        else if (strstr(msg, "sad") || strstr(msg, "fail") || strstr(msg, "pain"))
            negative++;

        else
            mixed++;
    }

    printf("[MFL🧠] Memory Stats → Positive:%d  Negative:%d  Mixed:%d\n", 
            positive, negative, mixed);

    // Stability weight calculation
    int stability = positive - negative;

    if (stability > 0) {
        printf("[MFL💛] Emotional Trend → Improving\n");
    } 
    else if (stability < 0) {
        printf("[MFL❤️‍🩹] Emotional Trend → Healing Required\n");
    } 
    else {
        printf("[MFL⚪] Emotional Trend → Neutral\n");
    }

    printf("[MFL✔️] Memory Fusion Complete.\n");
}
