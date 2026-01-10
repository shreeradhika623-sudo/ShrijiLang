#ifndef EVENT_H
#define EVENT_H

/*──────────────────────────────────────────────────────────────
 |  SHRIJILANG — EVENT SYSTEM
 |
 |  Purpose:
 |   • Represent what just happened in the system
 |   • Bridge Interpreter → State → AI
 |
 |  Introduced in STEP-6
 |  Extended in STEP-7.3 (COMMAND events)
 *────────────────────────────────────────────────────────────*/

/*──────────────────────────────────────────────────────────────
 |  EVENT TYPES
 *────────────────────────────────────────────────────────────*/
typedef enum {

    EVENT_NONE = 0,

    /* Execution lifecycle */
    EVENT_EXECUTION_START,
    EVENT_EXECUTION_BLOCKED,
    EVENT_EXECUTION_SUCCESS,

    /* Variable & logic */
    EVENT_ASSIGNMENT,
    EVENT_SHUNYA_ACCESS,

    /* Command system (STEP-7.3) */
    EVENT_COMMAND,          /* ls / cd / mkdir / aliases */

    /* Errors */
    EVENT_ERROR,
    EVENT_FATAL_ERROR

} EventType;


/*──────────────────────────────────────────────────────────────
 |  EVENT STRUCTURE
 *────────────────────────────────────────────────────────────*/
typedef struct {

    EventType type;

    /* Optional short context (safe capped) */
    char context[64];

} Event;


/*──────────────────────────────────────────────────────────────
 |  EVENT API
 *────────────────────────────────────────────────────────────*/
void event_fire(EventType type, const char *context);

#endif /* EVENT_H */
