#ifndef RUNTIME_H
#define RUNTIME_H

#include "state.h"
#include "value.h"

/*──────────────────────────────────────────────
 | SHRIJI RUNTIME — ISOLATED EXECUTION BRAIN
 | Each execution gets its own runtime state
 *──────────────────────────────────────────────*/

typedef struct {

    ExecutionState state;

    int break_flag;
    int continue_flag;
    int loop_depth;

    int return_flag;
    Value return_value;

} ShrijiRuntime;

/* Init once (fresh runtime) */
void runtime_init(ShrijiRuntime *rt);

/* Reset before each run */
void runtime_reset(ShrijiRuntime *rt);

#endif
