#ifndef SAKHI_H
#define SAKHI_H

#include "sakhi_states.h"

// -------------------------
//  FUNCTION DECLARATIONS
// -------------------------

// Entry point
const char* sakhi_bridge(const char *msg);

// Emotion detect
SakhiEmotion sakhi_emotion_detect(const char *msg);

// Memory
void sakhi_memory_store(const char *msg);
void sakhi_memory_update(SakhiState *st, SakhiEmotion emo);

// Healing
void sakhi_heal(SakhiEmotion emo, int bond);

// Anger protector
void sakhi_anger_if_needed(SakhiState *st, const char *msg);

// Personality shifts
void sakhi_personality_adapt(SakhiState *st);

// Prediction engine
void sakhi_predict(const char *msg, SakhiState *st);

// Core handler
void sakhi_core_handle(const char *msg, SakhiState *st);

// Level-9 NEW modules

SakhiContext sakhi_context_detect(const char *msg);

int sakhi_intensity_level(const char *msg);
const char* sakhi_response_engine(SakhiEmotion emo, SakhiContext ctx, int intensity);

// Core handler for response stage
void sakhi_core_handle_response(SakhiEmotion emo);

#endif
