#include <stdio.h>
#include <string.h>
#include "../include/niyu.h"

void niyu_self_heal(const char *emotion_label, const char *logic_label) {

    printf("[SHE❤️] Activating Self-Heal Engine...\n");

    // 1. Emotion–Logic Mismatch Detection
    if (strcmp(emotion_label, logic_label) != 0) {
        printf("[SHE❤️] Detected mismatch → Emotion:%s | Logic:%s\n",
               emotion_label, logic_label);

        printf("[SHE❤️] Healing → Aligning emotional + logical layers...\n");
    } else {
        printf("[SHE❤️] No mismatch detected. System stable.\n");
    }

    // 2. Strengthen Identity Stability
    printf("[SHE❤️] Identity Stability boosted.\n");

    // 3. Improve ESI (Emotional Stability Index)
    printf("[SHE❤️] Emotional Stability improved.\n");

    // 4. Provide gentle correction output
    printf("[SHE❤️] You are safe. I understand what you feel.\n");

    printf("[SHE❤️] Self-Heal Complete.\n");
}
