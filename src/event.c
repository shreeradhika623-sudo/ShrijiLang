#include <string.h>

#include "../include/event.h"
#include "../include/state.h"

/*──────────────────────────────────────────────────────────────
 |  SHRIJILANG — EVENT ENGINE
 |
 |  Maps events to state transitions
 |  STEP-6.3
 *────────────────────────────────────────────────────────────*/

/* External execution state (owned by interpreter) */
extern ExecutionState SHRIJI_STATE;


/*──────────────────────────────────────────────────────────────
 |  Fire an event into the system
 *────────────────────────────────────────────────────────────*/
void event_fire(EventType type, const char *context) {

    (void)context;  // context reserved for future use

    switch (type) {

        case EVENT_EXECUTION_SUCCESS:
        case EVENT_ASSIGNMENT:
            state_on_success(&SHRIJI_STATE);
            break;

        case EVENT_SHUNYA_ACCESS:
        case EVENT_ERROR:
            state_on_error(&SHRIJI_STATE);
            break;

        case EVENT_FATAL_ERROR:
            state_on_error(&SHRIJI_STATE);
            state_on_error(&SHRIJI_STATE);  // double impact
            break;

        case EVENT_EXECUTION_BLOCKED:
            SHRIJI_STATE.safety = STATE_CRITICAL;
            SHRIJI_STATE.human  = HUMAN_MANDATORY;
            break;

        case EVENT_EXECUTION_START:
        case EVENT_NONE:
        default:
            /* no-op */
            break;
    }
}
