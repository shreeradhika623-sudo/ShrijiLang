#ifndef SHRIJI_ENGINE_H
#define SHRIJI_ENGINE_H

#include "value.h"

/* ================= ENGINE STATUS ================= */

typedef enum {
    ENGINE_OK = 0,
    ENGINE_PARSE_ERROR,
    ENGINE_RUNTIME_ERROR
} EngineStatus;

typedef enum {
    OUTPUT_NONE = 0,
    OUTPUT_REPL,
    OUTPUT_EXPLICIT
} OutputMode;

/* ================= ENGINE RESULT ================= */

typedef struct {

    EngineStatus status;

    Value result;
   char explain_text[2048];
    OutputMode output_mode;
    /* optional intelligence fields */
    int was_fixed;
    int confidence_penalty;

    /* AST pointer (optional use) */
    void *ast;

} EngineResult;

/* ================= INIT ================= */

static inline void engine_result_init(EngineResult *res)
{
    if (!res) return;

res->status = ENGINE_OK;
res->output_mode = OUTPUT_NONE;
res->was_fixed = 0;
res->confidence_penalty = 0;
res->ast = NULL;
res->explain_text[0] = '\0';
}

#endif
