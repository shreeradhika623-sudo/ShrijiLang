#include "../include/runtime.h"
#include "../include/state.h"

void runtime_init(ShrijiRuntime *rt)
{
    if (!rt) return;

    state_init(&rt->state);

    rt->break_flag = 0;
    rt->continue_flag = 0;
    rt->loop_depth = 0;

    rt->return_flag = 0;
    rt->return_value = value_null();
}

void runtime_reset(ShrijiRuntime *rt)
{
    if (!rt) return;

    rt->break_flag = 0;
    rt->continue_flag = 0;
    rt->loop_depth = 0;

    rt->return_flag = 0;
    rt->return_value = value_null();

    rt->state.steps_used = 0;
}
