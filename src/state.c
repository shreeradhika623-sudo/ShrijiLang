#include <stdio.h>
#include "../include/state.h"

/*──────────────────────────────────────────────────────────────
 | SHRIJILANG — EXECUTION STATE ENGINE
 |
 | Purpose:
 |   - Runtime safety monitoring
 |   - Confidence / risk state management
 |   - Execution limits (anti-freeze)
 |   - Human confirmation escalation policy
 |
 | NOTE:
 |   This layer must remain deterministic.
 |   No external dependencies, no printing from core logic.
 *──────────────────────────────────────────────────────────────*/

/*──────────────────────────────────────────────
 | INTERNAL CONSTANTS (Tuning)
 *──────────────────────────────────────────────*/

/* Global execution budget:
 * Prevents accidental freeze when user writes an infinite loop.
 * Keep large enough so normal programs can run.
 */
#define SHRIJI_DEFAULT_MAX_STEPS   100000000LL  /* 10 crore */

/* Error escalation thresholds */
#define SHRIJI_WARN_THRESHOLD      3
#define SHRIJI_CRITICAL_THRESHOLD  5

/*──────────────────────────────────────────────
 | INIT
 *──────────────────────────────────────────────*/
void state_init(ExecutionState *state)
{
    if (!state) return;

    /* runtime safety status */
    state->safety = STATE_SAFE;

    /* confidence model */
    state->confidence = CONFIDENCE_HIGH;

    /* pressure model */
    state->error_pressure = 0;

    /* human control mode */
    state->human = HUMAN_AUTO_OK;

    /* execution budget */
    state->max_steps  = SHRIJI_DEFAULT_MAX_STEPS;
    state->steps_used = 0;
}

/*──────────────────────────────────────────────
 | RESET
 *──────────────────────────────────────────────*/
void state_reset(ExecutionState *state)
{
    state_init(state);
}

/*──────────────────────────────────────────────
 | ERROR HOOK
 *──────────────────────────────────────────────*/
void state_on_error(ExecutionState *state)
{
    if (!state) return;

    /* increase error pressure */
    if (state->error_pressure < 1000000)
        state->error_pressure++;

    /* stage-1: warning / risk */
    if (state->error_pressure >= SHRIJI_WARN_THRESHOLD) {
        state->confidence = CONFIDENCE_LOW;
        state->safety = STATE_RISK;
        state->human = HUMAN_CONFIRM_REQUIRED;
    }

    /* stage-2: critical stop */
    if (state->error_pressure >= SHRIJI_CRITICAL_THRESHOLD) {
        state->safety = STATE_CRITICAL;
        state->human = HUMAN_MANDATORY;
    }
}

/*──────────────────────────────────────────────
 | SUCCESS HOOK
 *──────────────────────────────────────────────*/
void state_on_success(ExecutionState *state)
{
    if (!state) return;

    /* decay pressure gradually */
    if (state->error_pressure > 0)
        state->error_pressure--;

    /* fully recover when stable again */
    if (state->error_pressure == 0) {
        state->confidence = CONFIDENCE_HIGH;
        state->safety = STATE_SAFE;
        state->human = HUMAN_AUTO_OK;
    }
}
