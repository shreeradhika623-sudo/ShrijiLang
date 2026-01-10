#include <stdio.h>
#include <string.h>
#include "../include/niyu.h"
#include "niyu_bridge.h"

/*
    ⭐ Niyu Bridge (Return-Based Version)
    - Internal logic stays same
    - Final reasoning string returned
    - Router + Tone Filter now fully compatible
*/

const char* niyu_bridge(const char *msg) {

    // STATIC BUFFER for return value
    static char final_out[512];
    memset(final_out, 0, sizeof(final_out));

    printf("[Niyu⚡] %s\n", msg);

    // SPECIAL COMMAND: memory dump
    if (strcmp(msg, "check memory") == 0) {
        niyu_memory_dump();
        snprintf(final_out, sizeof(final_out),
                 "Memory dump complete.");
        return final_out;
    }

    // --------------------------------
    // PREPARE tracking variables
    // --------------------------------
    char emotion_label[64] = "none";
    char logic_label[64]   = "none";

    // --------------------------------
    // 1. MEMORY ENGINE
    // --------------------------------
    niyu_memory_store(msg);

    // --------------------------------
    // 2. EMOTION ENGINE
    // --------------------------------
    niyu_emotion(msg);
    strcpy(emotion_label, niyu_emotion_last());

    // --------------------------------
    // 3. LOGIC ENGINE
    // --------------------------------
    niyu_logic_process(msg);
    strcpy(logic_label, niyu_logic_last());

    // --------------------------------
    // LEVEL 12 — SELF HEAL ENGINE CALL
    // --------------------------------
    niyu_self_heal(emotion_label, logic_label);
    niyu_self_optimize(msg);
    niyu_memory_fusion();

    // --------------------------------
    // FULL AI REASONING PIPELINE
    // --------------------------------
    niyu_observe(msg);
    niyu_deduce(msg);
    niyu_induce(msg);
    niyu_resolve(msg);
    niyu_synthesis(msg);
    niyu_meta_learn(msg);

    // LEVEL 4
    niyu_cause(msg);
    niyu_effect(msg);
    niyu_clarity(msg);

    // LEVEL 5
    niyu_self(emotion_label, logic_label);
    niyu_vision(msg);

    // LEVEL 6–7
    niyu_why_engine(emotion_label, logic_label);
    niyu_root_cause(msg);
    niyu_causal_chain(msg);
    niyu_depth_analysis(msg);

    // LEVEL 8–9
    niyu_stability(msg);
    niyu_anchor(msg);
    niyu_shield(msg);
    niyu_identity(msg);
    niyu_predict(msg);

    niyu_self_optimize(msg);
    niyu_consistency(msg);

    niyu_update_esi();

    // --------------------------------
    // BUILD FINAL OUTPUT MESSAGE
    // --------------------------------
    snprintf(final_out, sizeof(final_out),
        "Reasoning complete: emotion=%s, logic=%s.",
        emotion_label, logic_label
    );

    return final_out;
}
