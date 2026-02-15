#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/env.h"
#include "../include/error.h"

/*──────────────────────────────────────────────
 | SHRIJILANG — ENVIRONMENT IMPLEMENTATION (FINAL)
 |
 | Env = stack of scopes (EnvFrame)
 | frames[0] = global scope (always exists)
 |
 | RULES (LOCKED):
 |  - env_set     → current scope only
 |  - env_update  → nearest existing scope (top → bottom)
 |  - env_get     → nearest existing scope (top → bottom)
 |  - env_exists  → nearest existing scope (top → bottom)
 |
 | NOTE:
 |  - env_get returns OWNED Value (caller must free)
 *──────────────────────────────────────────────*/

/*──────────────────────────────────────────────
 | CREATE ENVIRONMENT
 *──────────────────────────────────────────────*/
Env *new_env(void)
{
    Env *env = (Env *)malloc(sizeof(Env));
    if (!env) {
        shriji_error(
            E_ASSIGN_01,
            "env",
            "memory allocation failed",
            "system out of memory"
        );
        return NULL;
    }

    memset(env, 0, sizeof(Env));

    /* initialize global scope */
    env->depth = 1;
    env->frames[0].count = 0;

    return env;
}

/*──────────────────────────────────────────────
 | SCOPE CONTROL
 *──────────────────────────────────────────────*/
void env_push_scope(Env *env)
{
    if (!env) return;

    if (env->depth >= ENV_MAX_SCOPES) {
        shriji_error(
            E_ASSIGN_01,
            "env",
            "scope limit exceeded",
            "too many nested blocks/functions"
        );
        return;
    }

    env->frames[env->depth].count = 0;
    env->depth++;
}

void env_pop_scope(Env *env)
{
    if (!env) return;

    /* never pop global scope */
    if (env->depth <= 1)
        return;

    EnvFrame *frame = &env->frames[env->depth - 1];

    /* free all values in this scope */
    for (int i = 0; i < frame->count; i++) {
        value_free(&frame->vars[i].value);
    }

    frame->count = 0;
    env->depth--;
}

/*──────────────────────────────────────────────
 | INTERNAL HELPER
 *──────────────────────────────────────────────*/
static EnvVar *find_in_frame(EnvFrame *frame, const char *name)
{
    if (!frame || !name) return NULL;

    for (int i = 0; i < frame->count; i++) {
        if (strcmp(frame->vars[i].name, name) == 0)
            return &frame->vars[i];
    }
    return NULL;
}

/*──────────────────────────────────────────────
 | VARIABLES
 *──────────────────────────────────────────────*/

/* create or overwrite in CURRENT scope only */
void env_set(Env *env, const char *name, Value v)
{
    if (!env || !name) return;

    EnvFrame *frame = &env->frames[env->depth - 1];

    EnvVar *var = find_in_frame(frame, name);
    if (var) {
        value_free(&var->value);
        var->value = value_copy(v);
        return;
    }

    if (frame->count >= ENV_MAX_VARS) {
        shriji_error(
            E_ASSIGN_01,
            name,
            "variable limit exceeded in scope",
            "reduce variables or increase ENV_MAX_VARS"
        );
        return;
    }

    strncpy(frame->vars[frame->count].name, name, 63);
    frame->vars[frame->count].name[63] = '\0';
    frame->vars[frame->count].value = value_copy(v);
    frame->count++;
}

/* update nearest existing scope */
void env_update(Env *env, const char *name, Value v)
{
    if (!env || !name) return;

    for (int d = env->depth - 1; d >= 0; d--) {
        EnvVar *var = find_in_frame(&env->frames[d], name);
        if (var) {
            value_free(&var->value);
            var->value = value_copy(v);
            return;
        }
    }

    /* not found → HARD FAILURE */
    shriji_error(
        E_ASSIGN_01,
        name,
        "variable not declared",
        "use: mavi x = ... first"
    );
}

/* read nearest scope */
Value env_get(Env *env, const char *name)
{
    if (!env || !name)
        return value_null();

    for (int d = env->depth - 1; d >= 0; d--) {
        EnvVar *var = find_in_frame(&env->frames[d], name);
        if (var)
            return value_copy(var->value);
    }

    /* HARD FAILURE */
    shriji_error(
        E_ASSIGN_01,
        name,
        "variable not defined",
        "declare variable before use"
    );

    return value_null();
}

/* existence check */
int env_exists(Env *env, const char *name)
{
    if (!env || !name)
        return 0;

    for (int d = env->depth - 1; d >= 0; d--) {
        if (find_in_frame(&env->frames[d], name))
            return 1;
    }

    return 0;
}
