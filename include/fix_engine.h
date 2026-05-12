/* RADHE_RADHE_SHREEJI */

/* =========================================================
   SHRIJILANG — EXECUTION ENGINE (CORE CONTRACT)
   ========================================================= */

#ifndef FIX_ENGINE_H
#define FIX_ENGINE_H

#include "ast.h"
#include "value.h"
#include "env.h"
#include "runtime.h"
#include "error.h"
#include "engine.h"

/* =========================================================
   ENGINE CONFIG
   ========================================================= */

/* Maximum number of fix attempts allowed */
#define ENGINE_MAX_FIX_ATTEMPTS 5

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

/* Free result safely */
void engine_result_free(EngineResult *res);

#endif /* FIX_ENGINE_H */
