/* SHREE_RADHIKA_RANI */

#include <stdio.h>
#include <string.h>

#include "../include/parser.h"
#include "../include/error.h"
#include "../include/fix_engine.h"
#include "../include/runtime.h"
#include "../include/value.h"
#include "../include/interpreter.h"
#include "../include/ast.h"
/*──────────────────────────────────────────────
   PARSE ONLY (STRICT MODE)
──────────────────────────────────────────────*/

static ASTNode *parse_once(const char *input)
{
    if (!input)
        return NULL;

    error_reported = 0;

    /* silent mode */
    shriji_set_error_mode(ERROR_MODE_COLLECT);

    ASTNode *node = parse_program(input);

    /* restore mode */
    shriji_set_error_mode(ERROR_MODE_IMMEDIATE);

    return node;
}

/*──────────────────────────────────────────────
   ENGINE RESULT INIT
──────────────────────────────────────────────*/

/*──────────────────────────────────────────────
   ENGINE RESULT FREE
──────────────────────────────────────────────*/

void engine_result_free(EngineResult *res)
{
    if (!res) return;

    if (res->result.type != VAL_NULL)
        value_free(&res->result);

    res->result = value_null();
}

/*──────────────────────────────────────────────
   MAIN EXECUTION ENGINE
──────────────────────────────────────────────*/

EngineResult shriji_engine_execute(const char *input, Env *env)
{
    EngineResult res;
    engine_result_init(&res);

    /* 🔒 RESET GLOBAL ERROR STATE */
    error_reported = 0;

    if (!input)
    {
        res.status = ENGINE_PARSE_ERROR;
        return res;
    }

    /* ================= PARSE ================= */

    ASTNode *ast = parse_once(input);

      res.ast = ast;

    if (!ast || error_reported)
    {
        res.status = ENGINE_PARSE_ERROR;
        return res;
    }

    /* ================= RUNTIME ================= */

    ShrijiRuntime runtime;
    runtime_init(&runtime);

    Value result = eval(ast, env, &runtime);

    if (runtime.error_flag || error_reported)
    {
        res.status = ENGINE_RUNTIME_ERROR;

        value_free(&result);
        ast_free(ast);

        return res;
    }

    /* ================= SUCCESS ================= */

    res.result = value_copy(result);

if (
    ast &&
    ast->type == AST_PROGRAM &&
    ast->stmt_count > 0
   )
   {
    ASTNode *expr = ast->statements[0];

    if (expr && expr->type == AST_COMMAND)
    {
    expr = expr->value;
    }

if (expr && expr->type == AST_BINARY)
{
    char op = expr->op[0];

    ASTNode *L = expr->left;
    ASTNode *R = expr->right;

    const char *label = "Expression executed successfully";

    if (op == '+')
        label = "Addition";

    else if (op == '-')
        label = "Subtraction";

    else if (op == '*')
        label = "Multiplication";

    else if (op == '/')
        label = "Division";

    if (
        L &&
        R &&
        L->type == AST_NUMBER &&
        R->type == AST_NUMBER
    )
    {

    if (result.type == VAL_BOOL)
{
    snprintf(
        res.explain_text,
        sizeof(res.explain_text),
        "%g %s %g = %s",
        L->number_value,
        expr->op,
        R->number_value,
        result.boolean ? "true" : "false"
    );
}
else
{
    snprintf(
        res.explain_text,
        sizeof(res.explain_text),
        "%g %s %g = %g",
        L->number_value,
        expr->op,
        R->number_value,
        result.number
    );
   }
 }
}
}
value_free(&result);

ast_free(ast);

res.status = ENGINE_OK;

return res;
}
