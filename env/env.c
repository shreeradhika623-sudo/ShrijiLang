#include <string.h>
#include <stdlib.h>

#include "../include/env.h"
#include "../include/value.h"

/*──────────────────────────────────────────────────────────────
 | SHRIJILANG — ENVIRONMENT STORE (SCOPE STACK)
 |
 | Responsibility:
 |   • Variable storage (Value)
 |   • Scope push / pop
 |   • Get / Set / Update / Exists
 |
 | RULES:
 |   • env NEVER prints
 |   • env NEVER calls shiri
 |   • Only DATA LAYER
 *──────────────────────────────────────────────────────────────*/

/*──────────────────────────────────────────────
 | CREATE NEW ENV
 *──────────────────────────────────────────────*/
Env *new_env(void)
{
    Env *env = (Env *)malloc(sizeof(Env));
    if (!env) return NULL;

    env->depth = 1; /* global scope exists */
    env->frames[0].count = 0;

    return env;
}

/*──────────────────────────────────────────────
 | PUSH NEW SCOPE
 *──────────────────────────────────────────────*/
void env_push_scope(Env *env)
{
    if (!env) return;

    if (env->depth >= ENV_MAX_SCOPES) {
        return; /* silently ignore overflow */
    }

    env->frames[env->depth].count = 0;
    env->depth++;
}

/*──────────────────────────────────────────────
 | POP SCOPE
 *──────────────────────────────────────────────*/
void env_pop_scope(Env *env)
{
    if (!env) return;

    /* never pop global scope */
    if (env->depth <= 1) return;

    int top = env->depth - 1;

    /* free all values in top scope */
    for (int i = 0; i < env->frames[top].count; i++) {
        value_free(&env->frames[top].vars[i].value);
    }

    env->frames[top].count = 0;
    env->depth--;
}

/*──────────────────────────────────────────────
 | CHECK IF VARIABLE EXISTS (search top → bottom)
 *──────────────────────────────────────────────*/
int env_exists(Env *env, const char *name)
{
    if (!env || !name) return 0;

    for (int f = env->depth - 1; f >= 0; f--) {
        for (int i = 0; i < env->frames[f].count; i++) {
            if (strcmp(env->frames[f].vars[i].name, name) == 0) {
                return 1;
            }
        }
    }

    return 0;
}

/*──────────────────────────────────────────────
 | SET VARIABLE
 | - if exists in CURRENT scope: update it
 | - else create new variable in CURRENT scope
 *──────────────────────────────────────────────*/
void env_set(Env *env, const char *name, Value v)
{
    if (!env || !name) return;

    int top = env->depth - 1;
    EnvFrame *frame = &env->frames[top];

    /* update if already exists in current scope */
    for (int i = 0; i < frame->count; i++) {
        if (strcmp(frame->vars[i].name, name) == 0) {
            value_free(&frame->vars[i].value);
            frame->vars[i].value = value_copy(v);
            return;
        }
    }

    /* add new variable in current scope */
    if (frame->count >= ENV_MAX_VARS) {
        return; /* silently ignore overflow */
    }

    strncpy(
        frame->vars[frame->count].name,
        name,
        sizeof(frame->vars[frame->count].name) - 1
    );

    frame->vars[frame->count].name[
        sizeof(frame->vars[frame->count].name) - 1
    ] = '\0';

    frame->vars[frame->count].value = value_copy(v);
    frame->count++;
}

/*──────────────────────────────────────────────
 | UPDATE VARIABLE (search top → bottom)
 | - if exists in ANY scope: update nearest found scope
 | - else create in current scope
 *──────────────────────────────────────────────*/
void env_update(Env *env, const char *name, Value v)
{
    if (!env || !name) return;

    /* search from top scope to global */
    for (int f = env->depth - 1; f >= 0; f--) {
        EnvFrame *frame = &env->frames[f];

        for (int i = 0; i < frame->count; i++) {
            if (strcmp(frame->vars[i].name, name) == 0) {
                value_free(&frame->vars[i].value);
                frame->vars[i].value = value_copy(v);
                return;
            }
        }
    }

    /* if not found, fallback: create in current scope */
    env_set(env, name, v);
}

/*──────────────────────────────────────────────
 | GET VARIABLE VALUE (search top → bottom)
 *──────────────────────────────────────────────*/
Value env_get(Env *env, const char *name)
{
    if (!env || !name) return value_null();

    for (int f = env->depth - 1; f >= 0; f--) {
        for (int i = 0; i < env->frames[f].count; i++) {
            if (strcmp(env->frames[f].vars[i].name, name) == 0) {
                return value_copy(env->frames[f].vars[i].value);
            }
        }
    }

    return value_null();
}
