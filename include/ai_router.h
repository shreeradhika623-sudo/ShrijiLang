#ifndef AI_ROUTER_H
#define AI_ROUTER_H

//──────────────────────────────────────────────────────────────
//  SHRIJILANG AI ROUTER — HEADER (UNIVERSE-CLASS VERSION)
//
//  This router directs AI messages to:
//      ✔ Sakhi  (Emotion / Healing)
//      ✔ Niyu   (Logic / Reasoning)
//      ✔ Shiri  (Soft Guidance)
//      ✔ Mira   (Knowledge / Computation)
//      ✔ Kavya  (Creativity / Inspiration)
//
//  Interpreter will call:
//      ai_router_process(message)
//──────────────────────────────────────────────────────────────


// MAIN PUBLIC FUNCTION
void ai_router_process(const char *msg);


// FUTURE EXPANSION HOOKS (V2)
// Enables:
//   - AI chaining
//   - AI fusion modes
//   - AI responsibility switching
//   - Custom AI personalities
//
// (Do not remove — needed for V2)
typedef enum {
    AI_MODE_AUTO = 0,        // default
    AI_MODE_LOGIC,
    AI_MODE_EMOTION,
    AI_MODE_GUIDANCE,
    AI_MODE_CREATIVE
} AI_Mode;

void ai_router_set_mode(AI_Mode mode);
AI_Mode ai_router_get_mode();

#endif
