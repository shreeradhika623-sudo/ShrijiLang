#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "user_config.h"

// Sakhi modules (no folder prefix needed!)
// Because Makefile already has: -Isakhi
#include "sakhi.h"
#include "sakhi_states.h"
#include "sakhi_context.h"
#include "sakhi_intensity.h"
#include "sakhi_response_engine.h"
#include "sakhi_tone.h"

// ----------------------------------------------------
// 🍀 QUICK MATH DETECTOR (5+5, 10+20, solve 5+5 etc.)
// ----------------------------------------------------
static int sakhi_simple_math(const char *msg, int *result) {

    int a, b;

    // Pattern: "5+5"
    if (sscanf(msg, "%d+%d", &a, &b) == 2) {
        *result = a + b;
        return 1;
    }

    // Pattern: "solve 5+5"
    if (strstr(msg, "solve") && sscanf(msg, "solve %d+%d", &a, &b) == 2) {
        *result = a + b;
        return 1;
    }

    return 0;
}

// ----------------------------------------------------
// 🌸 MAIN SAKHI BRIDGE
// ----------------------------------------------------
const char* sakhi_bridge(const char *msg) {

    static char final_out[512];
    memset(final_out, 0, sizeof(final_out));

    if (!msg || strlen(msg) == 0) {
        strcpy(final_out, "Papa… mujhe input nahi mila… ❤️");
        return final_out;
    }

    // ------------------------------------------------
    // 🍀 QUICK MATH HANDLING (INSIDE FUNCTION – FIXED)
    // ------------------------------------------------
    int math_result = 0;

    if (sakhi_simple_math(msg, &math_result)) {

        if (DEV_MODE) {
            snprintf(final_out, sizeof(final_out),
                "Papa… %s ka answer %d hota hai ❤️",
                msg, math_result
            );
        } else {
            snprintf(final_out, sizeof(final_out),
                "%s = %d",
                msg, math_result
            );
        }

        return final_out;
    }

    // ------------------------------------------------
    // 🌸 FULL EMOTION ENGINE PIPELINE
    // ------------------------------------------------

    // Step 1 — Emotion detect
    SakhiEmotion emo = sakhi_emotion_detect(msg);

    // Step 2 — Context detect
    SakhiContext ctx = sakhi_context_detect(msg);

    // Step 3 — Intensity detect
    int intensity = sakhi_intensity_level(msg);

    // Step 4 — Raw emotional output
    const char *raw = sakhi_response_engine(emo, ctx, intensity);

    // Step 5 — Tone filter (Papa… ❤️ output)
    const char *soft = sakhi_tone_filter(raw);

    snprintf(final_out, sizeof(final_out), "%s", soft);
    return final_out;
}
