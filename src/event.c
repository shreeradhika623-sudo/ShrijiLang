#include <string.h>

#include "../include/event.h"
#include "../include/state.h"
#include "../include/runtime.h"

/* Internal runtime binding */
static ShrijiRuntime *BOUND_RUNTIME = NULL;

/* Bind runtime once per execution */
void event_bind_runtime(ShrijiRuntime *rt)
{
    BOUND_RUNTIME = rt;
}

/* Fire event */
void event_fire(EventType type, const char *context)
{
    (void)context;

    if (!BOUND_RUNTIME)
        return;

    switch (type)
    {
        case EVENT_EXECUTION_SUCCESS:
        case EVENT_ASSIGNMENT:
            state_on_success(&BOUND_RUNTIME->state);
            break;

        case EVENT_SHUNYA_ACCESS:
        case EVENT_ERROR:
            state_on_error(&BOUND_RUNTIME->state);
            break;

        case EVENT_FATAL_ERROR:
            state_on_error(&BOUND_RUNTIME->state);
            state_on_error(&BOUND_RUNTIME->state);
            break;

        case EVENT_EXECUTION_BLOCKED:
            BOUND_RUNTIME->state.safety = STATE_CRITICAL;
            BOUND_RUNTIME->state.human  = HUMAN_MANDATORY;
            break;

        default:
            break;
    }
}
