#include <stdio.h>
#include "../include/state.h"

/*──────────────────────────────────────────────────────────────
 |  SHRIJILANG — STATE ENGINE IMPLEMENTATION
 *────────────────────────────────────────────────────────────*/

/*──────────────────────────────────────────────────────────────
 |  Initialize state to safe defaults
 *────────────────────────────────────────────────────────────*/
void state_init(ExecutionState *state) {
    if (!state) return;

    state->safety = STATE_SAFE;
    state->confidence = CONFIDENCE_HIGH;
    state->error_pressure = 0;
    state->human = HUMAN_AUTO_OK;

    state->max_steps = 1000000;   // 10 lakh operations allowed
    state->steps_used = 0;

}


/*──────────────────────────────────────────────────────────────
 |  Reset state completely
 *────────────────────────────────────────────────────────────*/
void state_reset(ExecutionState *state) {
    state_init(state);
}


/*──────────────────────────────────────────────────────────────
 |  Called when an error occurs
 *────────────────────────────────────────────────────────────*/
void state_on_error(ExecutionState *state) {
    if (!state) return;

    state->error_pressure++;

    if (state->error_pressure >= 3) {
        state->confidence = CONFIDENCE_LOW;
        state->safety = STATE_RISK;
        state->human = HUMAN_CONFIRM_REQUIRED;
    }

    if (state->error_pressure >= 5) {
        state->safety = STATE_CRITICAL;
        state->human = HUMAN_MANDATORY;
    }
}


/*──────────────────────────────────────────────────────────────
 |  Called when execution succeeds
 *────────────────────────────────────────────────────────────*/
void state_on_success(ExecutionState *state) {
    if (!state) return;

    if (state->error_pressure > 0)
        state->error_pressure--;

    if (state->error_pressure == 0) {
        state->confidence = CONFIDENCE_HIGH;
        state->safety = STATE_SAFE;
        state->human = HUMAN_AUTO_OK;
    }
}
