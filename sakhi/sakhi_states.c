#include "sakhi_states.h"

// -----------------------------
// INTERNAL GLOBAL SAKHI STATE
// -----------------------------
static SakhiState GLOBAL_SAKHI_STATE = {
    .bond_level = 50,
    .protect_instinct = 50,
    .last_emo = EMO_NEUTRAL,
    .mood = MOOD_STABLE
};

// -----------------------------------
// DEFAULT INITIALIZER
// -----------------------------------
SakhiState sakhi_default_state() {
    SakhiState st;
    st.bond_level = 50;
    st.protect_instinct = 50;
    st.last_emo = EMO_NEUTRAL;
    st.mood = MOOD_STABLE;
    return st;
}

// -----------------------------------
// GLOBAL ACCESSOR
// -----------------------------------
SakhiState* sakhi_get_state() {
    return &GLOBAL_SAKHI_STATE;
}
