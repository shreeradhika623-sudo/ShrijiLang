/* RADHE_RADHE_SHREEJI */

/* =========================================================
   SHRIJILANG — EXECUTION ENGINE (CORE CONTRACT)
   ========================================================= */

#ifndef SHRIJI_ENGINE_H
#define SHRIJI_ENGINE_H

#include "ast.h"
#include "value.h"
#include "env.h"
#include "runtime.h"
#include "error.h"

/* =========================================================
   ENGINE CONFIG
   ========================================================= */

/* Maximum number of fix attempts allowed */
#define ENGINE_MAX_FIX_ATTEMPTS 5

/* =========================================================
   ENGINE STATUS
   ========================================================= */

typedef enum
{
    ENGINE_OK = 0,
    ENGINE_PARSE_ERROR,
    ENGINE_RUNTIME_ERROR,
    ENGINE_FIX_FAILED
} EngineStatus;

/* =========================================================
   ENGINE RESULT STRUCTURE
   ========================================================= */

typedef struct
{
    /* ---------- Language Layer ---------- */

    ASTNode *ast;              /* Final AST (after fix if any) */

    /* ---------- Runtime Layer ---------- */

    Value result;              /* Final evaluated result */

    /* ---------- Fix Engine Meta ---------- */

    int was_fixed;             /* 1 if any fix applied */
    int fix_attempts;          /* number of attempts used */
    int confidence_penalty;    /* total penalty from fixes */

    /* ---------- Status ---------- */

    EngineStatus status;       /* final execution status */

} EngineResult;

/* =========================================================
   ENGINE ENTRY POINT
   ========================================================= */

/*
   Main execution pipeline:

   Input
     → Parse
     → Fix (if needed)
     → Re-parse
     → Runtime
     → Result

   NOTE:
   - This function is PURE (no printing ideally in future)
   - KRST should control how it is used
*/
EngineResult shriji_engine_execute(
    const char *input,
    Env *env
);

/* =========================================================
   LOW-LEVEL CONTROL (INTERNAL USE)
   ========================================================= */

/* Single parse attempt (no fix) */
ASTNode *engine_parse_once(const char *input);

/* Apply fix rules */
int engine_apply_fix(
    char *buffer,
    size_t size,
    const ShrijiErrorInfo *err
);

/* =========================================================
   SAFETY UTILITIES
   ========================================================= */

/* Reset result safely */
void engine_result_init(EngineResult *res);

/* Free result safely */
void engine_result_free(EngineResult *res);

/* =========================================================
   LEGACY FIX ENGINE (KRST COMPAT)
   ========================================================= */

ASTNode *language_execute_with_fix(
    const char *input,
    int *was_fixed,
    int *penalty_out
);
typedef enum {
    FIX_NONE = 0,
    FIX_SAFE,
    FIX_DOUBTFUL,
    FIX_REJECT
} FixType;

FixType fix_apply(const char *input, char *output);

#endif /* SHRIJI_ENGINE_H */
