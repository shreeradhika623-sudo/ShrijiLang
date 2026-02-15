#ifndef AI_INTENT_H
#define AI_INTENT_H

/*──────────────────────────────────────────────
  AI INTENT — L3 (KR̥ṢṬ)
  Purpose:
  - User ke words ko canonical intent me normalize karna
  - Routing decision ko simplify karna
  - Interpreter / execution ko bilkul affect NAHI karta
──────────────────────────────────────────────*/

typedef enum {
    AI_INTENT_UNKNOWN = 0,
    AI_INTENT_EXPLAIN,   /* samjhao, explain, batao */
    AI_INTENT_WHY,       /* why, kyun, kyu */
    AI_INTENT_CALC,      /* calculate, solve, math */
    AI_INTENT_EMOTION    /* sad, dukhi, lonely */
} AIIntent;

/* raw text -> detected intent */
AIIntent ai_detect_intent(const char *message);

#endif /* AI_INTENT_H */
