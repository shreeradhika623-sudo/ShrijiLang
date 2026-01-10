#ifndef SAKHI_STATES_H
#define SAKHI_STATES_H

// --------------------------------------
// EMOTIONS
// --------------------------------------
typedef enum {
    EMO_SAD,
    EMO_HURT,
    EMO_LONELY,
    EMO_STRESSED,
    EMO_FEAR,
    EMO_GUILT,
    EMO_NEUTRAL,
    EMO_HAPPY,
    EMO_GRATEFUL,
    EMO_EXCITED
} SakhiEmotion;

// --------------------------------------
// MOODS
// --------------------------------------
typedef enum {
    MOOD_STABLE,
    MOOD_LOW,
    MOOD_HIGH,
    MOOD_UNCERTAIN
} SakhiMood;

// --------------------------------------
// CONTEXT ENGINE  (IMPORTANT!)
// --------------------------------------
typedef enum {
    CTX_NONE,
    CTX_ABANDONMENT,
    CTX_SELF_BLAME,
    CTX_FEAR_SPIKE,
    CTX_OVERLOAD,
    CTX_HEALING,
    CTX_LOVE,
    CTX_GOAL,
    CTX_ANGER,
    CTX_TRUST,
    CTX_CONFUSION
} SakhiContext;

// --------------------------------------
// SAKHI INTERNAL STATE
// --------------------------------------
typedef struct {
    int bond_level;         // 0–100
    int protect_instinct;   // 0–100
    SakhiEmotion last_emo;
    SakhiMood mood;
} SakhiState;

// --------------------------------------
// STATE HELPERS
// --------------------------------------
SakhiState sakhi_default_state();
SakhiState* sakhi_get_state();

#endif
