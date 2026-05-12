#include "../include/runtime.h"

#include "../include/engine.h"

#include "../include/state.h"
#include "../include/value.h"

/* GLOBAL POINTER */
ShrijiRuntime *current_runtime = NULL;

/*──────────────────────────────────────────────
 | INIT
 *──────────────────────────────────────────────*/
void runtime_init(ShrijiRuntime *rt)
{
    if (!rt) return;

    current_runtime = rt;

    state_init(&rt->state);

    rt->break_flag = 0;
    rt->continue_flag = 0;
    rt->loop_depth = 0;

    rt->return_flag = 0;
    rt->return_value = value_null();

    rt->call_depth = 0;

    rt->error_flag = 0;

    rt->printed = 0;

    rt->executed = 0;

    rt->last_output_mode = OUTPUT_NONE;
}

/*──────────────────────────────────────────────
 | RESET (per execution)
 *──────────────────────────────────────────────*/
void runtime_reset(ShrijiRuntime *rt)
{
    if (!rt) return;

    rt->break_flag = 0;
    rt->continue_flag = 0;
    rt->loop_depth = 0;

    rt->return_flag = 0;
    rt->return_value = value_null();

    rt->state.steps_used = 0;

    rt->call_depth = 0;

    rt->error_flag = 0;

    rt->printed = 0;

    rt->executed = 0;

    rt->last_output_mode = OUTPUT_NONE;

}
