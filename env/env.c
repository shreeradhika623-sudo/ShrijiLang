#include <string.h>
#include <stdlib.h>

#include "../include/env.h"

/*──────────────────────────────────────────────────────────────
 | SHRIJILANG — ENVIRONMENT STORE (FINAL V1)
 |
 | Responsibility:
 |   • Variable storage
 |   • Check existence
 |   • Get / Set value
 |
 | RULES:
 |   • env NEVER prints
 |   • env NEVER calls shiri
 |   • env NEVER decides philosophy
 |   • Only DATA LAYER
 *──────────────────────────────────────────────────────────────*/


//──────────────────────────────────────────────
// CREATE NEW ENV
//──────────────────────────────────────────────
Env *new_env(void) {

    Env *env = (Env *)malloc(sizeof(Env));
    if (!env) return NULL;

    env->count = 0;
    return env;
}


//──────────────────────────────────────────────
// CHECK IF VARIABLE EXISTS
//──────────────────────────────────────────────
int env_exists(Env *env, const char *name) {

    if (!env || !name) return 0;

    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            return 1;
        }
    }

    return 0;
}


//──────────────────────────────────────────────
// SET VARIABLE (CREATE / UPDATE)
//──────────────────────────────────────────────
void env_set(Env *env, const char *name, long long value) {

    if (!env || !name) return;

    // update if already exists
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            env->vars[i].value = value;
            return;
        }
    }

    // add new variable
    if (env->count >= ENV_MAX_VARS) {
        return; // silently ignore overflow
    }

    strncpy(
        env->vars[env->count].name,
        name,
        sizeof(env->vars[env->count].name) - 1
    );

    env->vars[env->count].name[
        sizeof(env->vars[env->count].name) - 1
    ] = '\0';

    env->vars[env->count].value = value;
    env->count++;
}


//──────────────────────────────────────────────
// GET VARIABLE VALUE
// (assumes existence already checked)
//──────────────────────────────────────────────
long long env_get(Env *env, const char *name) {

    if (!env || !name) return 0;

    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            return env->vars[i].value;
        }
    }

    // SHOULD NOT HAPPEN if env_exists() used correctly
    return 0;
}
