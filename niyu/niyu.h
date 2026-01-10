#ifndef NIYU_H
#define NIYU_H

// CORE BRIDGE
void niyu_bridge(const char *msg);

// MEMORY ENGINE
void niyu_memory_store(const char *msg);
const char* niyu_last_message();
extern const char *niyu_last_message_global;

#define NIYU_MEMORY_LIMIT 251

typedef struct {
    char entries[NIYU_MEMORY_LIMIT][512];
    int write_index;
    int total;
} NiyuMemory;

extern NiyuMemory niyu_memory;

const char* niyu_memory_get(int index);
int niyu_memory_count();

// EMOTION ENGINE
extern int emotion_score;
void niyu_emotion(const char *msg);
const char* niyu_emotion_last();

// LOGIC ENGINE
void niyu_logic_process(const char *msg);
const char* niyu_logic_last();

// COGNITION MODULES
void niyu_observe(const char *msg);
void niyu_deduce(const char *msg);
void niyu_induce(const char *msg);
void niyu_resolve(const char *msg);
void niyu_synthesis(const char *msg);
void niyu_meta_learn(const char *msg);
void niyu_consistency(const char *msg);

// WHY ENGINE
void niyu_reason_logic(const char *logic_label);
void niyu_reason_emotion(const char *emotion_label);
void niyu_why_engine(const char *emotion_label, const char *logic_label);

// CAUSE–EFFECT
void niyu_cause(const char *msg);
void niyu_effect(const char *msg);
void niyu_clarity(const char *msg);

// SELF AWARENESS
void niyu_self(const char *emotion_label, const char *logic_label);
void niyu_vision(const char *msg);

// LEVEL 8
void niyu_stability(const char *msg);
void niyu_anchor(const char *msg);
void niyu_shield(const char *msg);

// LEVEL 9
void niyu_identity(const char *msg);
void niyu_predict(const char *msg);

// GLOBAL IDENTITY VARIABLES
extern int positive_identity;
extern int negative_identity;
extern int mixed_identity;

// LEVEL 9 ROOT
void niyu_root_cause(const char *msg);
void niyu_causal_chain(const char *msg);
void niyu_depth_analysis(const char *msg);

// LEVEL 10
void niyu_update_esi();
int niyu_get_esi();
extern int global_esi;

void niyu_memory_dump();
#endif
