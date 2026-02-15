#ifndef NIYU_H
#define NIYU_H

// ============================================================
// CORE BRIDGE
// ============================================================
const char* niyu_bridge(const char *msg);


// ============================================================
// MEMORY ENGINE v2.51 — 251 MESSAGE BUFFER
// ============================================================
#define NIYU_MEMORY_LIMIT 251

typedef struct {
    char entries[NIYU_MEMORY_LIMIT][512];
    int write_index;
    int total;
} NiyuMemory;

// GLOBAL MEMORY OBJECT
extern NiyuMemory niyu_memory;

// GLOBAL POINTER FOR CONSISTENCY ENGINE
extern const char *niyu_last_message_global;

// MEMORY FUNCTIONS
void niyu_memory_store(const char *msg);
const char* niyu_memory_get(int index);
const char* niyu_last_message();
int niyu_memory_count();


// ============================================================
// EMOTION ENGINE
// ============================================================
extern int emotion_score;

void niyu_emotion(const char *msg);
const char* niyu_emotion_last();

// ============================================================
// LOGIC ENGINE
// ============================================================
void niyu_logic_process(const char *msg);
const char* niyu_logic_last();


// ============================================================
// COGNITION MODULES
// ============================================================
void niyu_observe(const char *msg);
void niyu_deduce(const char *msg);
void niyu_induce(const char *msg);
void niyu_resolve(const char *msg);
void niyu_synthesis(const char *msg);
void niyu_meta_learn(const char *msg);
void niyu_consistency(const char *msg);


// ============================================================
// REASONING / WHY ENGINE
// ============================================================
void niyu_reason_logic(const char *logic_label);
void niyu_reason_emotion(const char *emotion_label);
void niyu_why_engine(const char *emotion_label, const char *logic_label);


// ============================================================
// CAUSE–EFFECT MODULES
// ============================================================
void niyu_cause(const char *msg);
void niyu_effect(const char *msg);
void niyu_clarity(const char *msg);


// ============================================================
// SELF-AWARENESS MODULES
// ============================================================
void niyu_self(const char *emotion, const char *logic_class);
void niyu_vision(const char *msg);


// ============================================================
// LEVEL-8 MODULES (Stability Engine)
// ============================================================
void niyu_stability(const char *msg);
void niyu_anchor(const char *msg);
void niyu_shield(const char *msg);


// ============================================================
// LEVEL-9 MODULES (Identity Engine + Depth Engine)
// ============================================================
void niyu_identity(const char *msg);
void niyu_predict(const char *msg);

extern int positive_identity;
extern int negative_identity;
extern int mixed_identity;

void niyu_root_cause(const char *msg);
void niyu_causal_chain(const char *msg);
void niyu_depth_analysis(const char *msg);


// ============================================================
// LEVEL-10 MODULES (ESI Engine)
// ============================================================
void niyu_update_esi();
int niyu_get_esi();
extern int global_esi;

void niyu_memory_dump();


// ============================================================
// LEVEL-12 MODULES (NEW — declarations only)
// ============================================================
void niyu_self_heal(const char *emotion_label, const char *logic_label);
void niyu_self_optimize(const char *msg);
void niyu_memory_fusion();

// --------------------------------
// LEVEL 12 — ADVANCED MODULES
// --------------------------------
void niyu_self_heal(const char *emotion_label, const char *logic_label);
void niyu_self_optimize(const char *msg);
void niyu_memory_fusion();

#endif
