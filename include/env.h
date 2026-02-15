#ifndef ENV_H
#define ENV_H

#include "value.h"

/*──────────────────────────────────────────────
 | SHRIJILANG — ENVIRONMENT (VARIABLE STORE)
 |
 | Env = Scope stack
 |  - Global scope always exists
 |  - Blocks { } push/pop scopes
 |
 | Rules:
 |  - env_set()     : create/update in CURRENT scope only
 |  - env_update()  : update in nearest existing scope (top → bottom)
 |  - env_get()     : read from nearest scope (top → bottom)
 |  - env_exists()  : check existence (top → bottom)
 *──────────────────────────────────────────────*/

#define ENV_MAX_VARS     1024
#define ENV_MAX_SCOPES   64

/* one variable */
typedef struct {
    char name[64];
    Value value;
} EnvVar;

/* one scope frame */
typedef struct {
    EnvVar vars[ENV_MAX_VARS];
    int count;
} EnvFrame;

/* environment = stack of frames */
typedef struct {
    EnvFrame frames[ENV_MAX_SCOPES];
    int depth; /* >= 1 always (global scope) */
} Env;

/* create */
Env *new_env(void);

/* scope control */
void env_push_scope(Env *env);
void env_pop_scope(Env *env);

/* variables */
void env_set(Env *env, const char *name, Value v);       /* current scope only */
void env_update(Env *env, const char *name, Value v);    /* nearest scope update */
Value env_get(Env *env, const char *name);               /* nearest scope read */
int env_exists(Env *env, const char *name);

#endif /* ENV_H */
