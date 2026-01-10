#ifndef STATE_H
#define STATE_H

/*──────────────────────────────────────────────────────────────
 |  SHRIJILANG — EXECUTION STATE SYSTEM
 |
 |  Purpose:
 |   • Track current execution condition
 |   • Provide memory of risk, confidence, and errors
 |
 |  Used by:
 |   • Interpreter
 |   • Decision Engine (future)
 *────────────────────────────────────────────────────────────*/

/*──────────────────────────────────────────────────────────────
 |  SAFETY LEVEL — how risky the current situation is
 *────────────────────────────────────────────────────────────*/
typedef enum {
    STATE_SAFE = 0,
    STATE_ALERT,
    STATE_RISK,
    STATE_CRITICAL
} SafetyLevel;


/*──────────────────────────────────────────────────────────────
 |  CONFIDENCE LEVEL — how sure the system is
 *────────────────────────────────────────────────────────────*/
typedef enum {
    CONFIDENCE_HIGH = 0,
    CONFIDENCE_MEDIUM,
    CONFIDENCE_LOW
} ConfidenceLevel;


/*──────────────────────────────────────────────────────────────
 |  HUMAN DEPENDENCY — when human input is required
 *────────────────────────────────────────────────────────────*/
typedef enum {
    HUMAN_AUTO_OK = 0,
    HUMAN_CONFIRM_REQUIRED,
    HUMAN_MANDATORY
} HumanDependency;


/*──────────────────────────────────────────────────────────────
 |  STATE STRUCTURE — complete execution snapshot
 *────────────────────────────────────────────────────────────*/
typedef struct {
    SafetyLevel      safety;
    ConfidenceLevel  confidence;
    int              error_pressure;
    HumanDependency  human;
long long max_steps;
long long steps_used;

} ExecutionState;


/*──────────────────────────────────────────────────────────────
 |  STATE API (implemented in state.c)
 *────────────────────────────────────────────────────────────*/
void state_init(ExecutionState *state);
void state_reset(ExecutionState *state);
void state_on_error(ExecutionState *state);
void state_on_success(ExecutionState *state);

#endif /* STATE_H */
