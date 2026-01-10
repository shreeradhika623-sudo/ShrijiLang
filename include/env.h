#ifndef ENV_H
#define ENV_H

#define ENV_MAX_VARS 1024

typedef struct {
    char name[64];
    long long value;
} EnvVar;

typedef struct {
    EnvVar vars[ENV_MAX_VARS];
    int count;
} Env;

// create environment
Env *new_env(void);

// set variable
void env_set(Env *env, const char *name, long long value);

// get variable (assumes exists)
long long env_get(Env *env, const char *name);

// 🔥 ADD THIS (NEW)
int env_exists(Env *env, const char *name);

#endif
