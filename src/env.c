#include <string.h>
#include <stdlib.h>

#include "../include/env.h"
#include "../include/error.h"

/*──────────────────────────────────────────────────────────────
 | SHRIJILANG — ENVIRONMENT STORE (FINAL • LOCKED)
 |
 | CORE LAW:
 |   • Undefined variable = HARD FAILURE
 |   • No fake value
 |   • No fallback 0
 |   • Caller MUST stop execution
 *──────────────────────────────────────────────────────────────*/

// Global runtime error flag
int SHRIJI_RUNTIME_ERROR = 0;

Env *new_env(void) {
    Env *env = malloc(sizeof(Env));
    if (!env) {
        shriji_error(
            E_ASSIGN_01,
            "env",
            "memory allocation failed",
            "system out of memory"
        );
        SHRIJI_RUNTIME_ERROR = 1;
        return NULL;
    }

    env->count = 0;
    return env;
}

void env_set(Env *env, const char *name, long long value) {
    if (!env || !name) return;

    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            env->vars[i].value = value;
            return;
        }
    }

    if (env->count >= ENV_MAX_VARS) {
        shriji_error(
            E_ASSIGN_01,
            "env",
            "variable limit exceeded",
            "reduce variables or increase ENV_MAX_VARS"
        );
        SHRIJI_RUNTIME_ERROR = 1;
        return;
    }

    strncpy(env->vars[env->count].name,
            name,
            sizeof(env->vars[env->count].name) - 1);

    env->vars[env->count].name[
        sizeof(env->vars[env->count].name) - 1
    ] = '\0';

    env->vars[env->count].value = value;
    env->count++;
}

long long env_get(Env *env, const char *name) {
    if (!env || !name) {
        SHRIJI_RUNTIME_ERROR = 1;
        return 0;
    }

    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            return env->vars[i].value;
        }
    }

    // HARD FAILURE — undefined variable
    shriji_error(
        E_ASSIGN_01,
        name,
        "variable not defined",
        "declare variable using: mavi x = ..."
    );

    SHRIJI_RUNTIME_ERROR = 1;
    return 0; // value ignored by caller
}
