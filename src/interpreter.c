#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#include "../include/interpreter.h"
#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/env.h"
#include "../include/error.h"
#include "../include/state.h"
#include "../include/event.h"
#include "../include/command.h"
#include "../include/value.h"
#include "../include/ai_router.h"

#include "../include/smriti_personal.h"
#include "../include/smriti_session.h"

#include "../include/runtime.h"

#include "../include/user_config.h"
#include "../include/env.h"

#ifdef SHRIJI_ENABLE_MASTER_TOKENS
#include "lang/token_master.h"
#endif

/*──────────────────────────────────────────────
  helper: unescape string like "a\\nb" -> actual newline
  supports: \n \t \r \\ \"
──────────────────────────────────────────────*/
static char *unescape_string(const char *s)
{
    if (!s) return NULL;

    size_t n = strlen(s);
    char *out = (char *)malloc(n + 1);
    if (!out) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < n; i++) {
        if (s[i] == '\\' && i + 1 < n) {
            char c = s[i + 1];
            if (c == 'n')      { out[j++] = '\n'; i++; continue; }
            else if (c == 't') { out[j++] = '\t'; i++; continue; }
            else if (c == 'r') { out[j++] = '\r'; i++; continue; }
            else if (c == '\\'){ out[j++] = '\\'; i++; continue; }
            else if (c == '"') { out[j++] = '"';  i++; continue; }
        }
        out[j++] = s[i];
    }

    out[j] = '\0';
    return out;
}
/*──────────────────────────────────────────────────────────────
 | SHRIJI TRUTH DIARY — SINGLE SOURCE OF FACT
 *──────────────────────────────────────────────────────────────*/
ShrijiTruthDiary SHRIJI_DIARY = {0};

/*──────────────────────────────────────────────────────────────
 | STATE DECISION HOOK
 *──────────────────────────────────────────────────────────────*/
static int state_allows_execution(ShrijiRuntime *rt)
{
    if (!rt) return 0;

    if (rt->state.safety == STATE_CRITICAL) {
        return 0;
    }
    return 1;
}

/*──────────────────────────────────────────────────────────────
 | SAFE DIVISION
 *──────────────────────────────────────────────────────────────*/
static double safe_div(double a, double b)

{
    if (b == 0) {
        event_fire(EVENT_ERROR, "division by zero");
        shriji_error(
            E_RUNTIME_DIV_ZERO,
            "division",
            "division by zero",
            "denominator must not be zero"
        );
        return 0;
    }
    return a / b;
}

/*──────────────────────────────────────────────────────────────
 | SAFE MODULO
 *──────────────────────────────────────────────────────────────*/
static double safe_mod(double a, double b)
{
    if (b == 0) {
        event_fire(EVENT_ERROR, "modulo by zero");
        shriji_error(
            E_RUNTIME_DIV_ZERO,
            "modulo",
            "modulo by zero",
            "denominator must not be zero"
        );
        return 0;
    }

    long long ai = (long long)a;
    long long bi = (long long)b;

    if (bi == 0)
        return 0;

    return (double)(ai % bi);
}

/*──────────────────────────────────────────────────────────────
 | Value → number helper
 *──────────────────────────────────────────────────────────────*/
static const char *strip_quotes(const char *s)
{
    if (!s) return "";

    while (*s == ' ' || *s == '\t')
        s++;

    size_t len = strlen(s);

    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        static char buf[1024];
        size_t n = len - 2;
        if (n >= sizeof(buf)) n = sizeof(buf) - 1;

        memcpy(buf, s + 1, n);
        buf[n] = '\0';
        return buf;
    }

    return s;
}
/*──────────────────────────────────────────────────────────────
 | MAIN EVALUATOR
 *──────────────────────────────────────────────────────────────*/
Value shriji_execute_line(const char *line, Env *env, ShrijiRuntime *rt)
{
    if (!line || !*line) return value_null();

    while (*line == ' ' || *line == '\t') line++;
    if (*line == '\0') return value_null();

    if (DEV_MODE)
        printf("[KRST] Input: %s\n", line);

    /*  FIXED: wrapper use karo */
    ASTNode *node = parse_program(line);

    if (!node || error_reported)
    {
        if (rt) {
            rt->error_flag = 1;
        }

        error_reported = 0;
        return value_null();
    }

    return eval(node, env, rt);
}


Value eval(ASTNode *node, Env *env, ShrijiRuntime *rt)
{
    static int state_initialized = 0;

    if (!state_initialized) {
        state_init(&rt->state);
        state_initialized = 1;
    }

    if (!state_allows_execution(rt) || !node)
        return value_null();

    /* 🧠 GLOBAL STEP COUNTER */
    if (node->type != AST_WHILE &&
        node->type != AST_BREAK &&
        node->type != AST_CONTINUE &&
        node->type != AST_BLOCK &&
        node->type != AST_PROGRAM) {
        rt->state.steps_used++;
    }

    if (rt->state.steps_used > rt->state.max_steps) {
        shriji_error(
            E_PARSE_02,
            "runtime",
            "execution limit exceeded",
            "possible infinite loop detected"
        );
        rt->state.safety = STATE_CRITICAL;
        return value_null();
    }



switch (node->type) {

case AST_BREAK:
    if (rt->loop_depth <= 0) {
        shriji_error(
            E_RUNTIME_01,
            "break",
            "loop ke bahar rukja allowed nahi hai",
            "rukja sirf while loop ke andar use karein"
        );
        return value_null();
    }

    rt->break_flag = 1;
    return value_null();

case AST_CONTINUE:

     if (rt->loop_depth <= 0) {
        shriji_error(
        E_RUNTIME_01,
        "continue",
        "loop ke bahar continue allowed nahi hai",
        "continue sirf while loop ke andar use karein"
    );
    return value_null();
}
    rt->continue_flag = 1;
    return value_null();


case AST_RETURN: {
        Value v = value_null();
        if (node->return_expr)
         v = eval(node->return_expr, env, rt);

       rt->return_flag = 1;
       rt->return_value = v;
        return v;
    }

case AST_UNARY:
{
    ASTNode *cur = node;
    int sign = 1;

    while (cur && cur->type == AST_UNARY) {
        if (cur->op[0] == '-')
            sign *= -1;

        cur = cur->left;
    }

    Value v = eval(cur, env, rt);

    if (v.type != VAL_NUMBER)
        return value_null();

    v.number = v.number * sign;

    return v;
}

/*────────────────────────────────────────
 | INC / DEC (++ / --)
 *────────────────────────────────────────*/

case AST_INC: {

    /* only identifiers allowed */
    if (!node->left || node->left->type != AST_IDENTIFIER) {
        return value_null();
    }

    const char *name = node->left->name;

    Value old = env_get(env, name);

    if (old.type != VAL_NUMBER) {
        return value_null();
    }

    Value newv = old;
    newv.number = old.number + 1;

    env_set(env, name, newv);

    if (node->is_prefix)
        return newv;
    else
        return old;
}

case AST_DEC: {

    /* only identifiers allowed */
    if (!node->left || node->left->type != AST_IDENTIFIER) {
        return value_null();
    }

    const char *name = node->left->name;

    Value old = env_get(env, name);

    if (old.type != VAL_NUMBER) {
        return value_null();
    }

    Value newv = old;
    newv.number = old.number - 1;

    env_set(env, name, newv);

    if (node->is_prefix)
        return newv;
    else
        return old;
}

    /*──────────────────────────────────────────────
      FUNCTIONS
    ──────────────────────────────────────────────*/

    case AST_FUNCTION: {
        /* store function AST node into env */
        env_set(env, node->function_name, value_function(node));
        return value_null();
    }

case AST_RACHNA: {
    env_set(env, node->rachna_name, value_function(node));
    return value_null();
}

case AST_CALL: {

/*────────────────────────────
  STACK GUARD (ENTRY)
────────────────────────────*/
rt->call_depth++;

if (rt->call_depth > 3000) {
    shriji_error(
        E_RUNTIME_01,
        "recursion",
        "maximum recursion depth exceeded",
        "safe limit: 3000"
    );

    rt->call_depth--;
    return value_null();
}

   /*──────────────────────────────────────────────
      BUILT-IN FUNCTIONS (FILE I/O)
      padho("file")                -> returns string
      likho("file", "text")        -> overwrite write -> returns 1/0
      jodo("file", "text")         -> append write   -> returns 1/0

      ✅ polish:
      - supports escape sequences: \n \t \r \\ \"
    ──────────────────────────────────────────────*/

    /* READ */
    if (strcmp(node->function_name, "padho") == 0) {

        if (node->arg_count != 1) {
            shriji_error(
                E_PARSE_02,
                "padho",
                "argument count mismatch",
                "use: padho(\"file.txt\")"
            );
            return value_null();
        }

        Value pathv = eval(node->args[0], env, rt);

        if (pathv.type != VAL_STRING || !pathv.string) {
            value_free(&pathv);
            shriji_error(
                E_PARSE_02,
                "padho",
                "file path must be string",
                "use: padho(\"file.txt\")"
            );
            return value_null();
        }

        const char *path = strip_quotes(pathv.string);

FILE *fp = fopen(path, "rb");
if (!fp) {
    value_free(&pathv);

    shriji_error(
        E_PARSE_02,
        "padho",
        "file not found",
        "check path or create file first"
    );

    return value_string("");
}
        fseek(fp, 0, SEEK_END);
        long sz = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        if (sz < 0) sz = 0;
        if (sz > 1024 * 1024) sz = 1024 * 1024; /* 1MB limit v1 */

        char *buf = (char *)malloc((size_t)sz + 1);
        if (!buf) {
            fclose(fp);
            value_free(&pathv);
            return value_null();
        }

        size_t nread = fread(buf, 1, (size_t)sz, fp);
        buf[nread] = '\0';

        fclose(fp);
        value_free(&pathv);

        Value out = value_string(buf);
        free(buf);
        return out;
    }

    /* WRITE (overwrite) */
    if (strcmp(node->function_name, "likho") == 0) {

        if (node->arg_count != 2) {
            shriji_error(
                E_PARSE_02,
                "likho",
                "argument count mismatch",
                "use: likho(\"file.txt\", \"data\")"
            );
            return value_null();
        }

        Value pathv = eval(node->args[0], env, rt);
        Value datav = eval(node->args[1], env, rt);

        if (pathv.type != VAL_STRING || !pathv.string) {
            value_free(&pathv);
            value_free(&datav);
            shriji_error(
                E_PARSE_02,
                "likho",
                "file path must be string",
                "use: likho(\"file.txt\", \"data\")"
            );
            return value_null();
        }

        const char *path = strip_quotes(pathv.string);

        const char *data = "";
        char tmp[128];
        tmp[0] = '\0';

        char *tmp_data = NULL;

        if (datav.type == VAL_STRING && datav.string) {
            tmp_data = unescape_string(strip_quotes(datav.string));
            data = tmp_data ? tmp_data : "";
        } else if (datav.type == VAL_NUMBER) {
            snprintf(tmp, sizeof(tmp), "%g", datav.number);
            data = tmp;
        }

        FILE *fp = fopen(path, "wb");
        if (!fp) {
            if (tmp_data) free(tmp_data);
            value_free(&pathv);
            value_free(&datav);
            return value_number(0);
        }

        fwrite(data, 1, strlen(data), fp);
        fclose(fp);

        if (tmp_data) free(tmp_data);

        value_free(&pathv);
        value_free(&datav);

        return value_number(1);
    }

    /* APPEND */
    if (strcmp(node->function_name, "jodo") == 0) {

        if (node->arg_count != 2) {
            shriji_error(
                E_PARSE_02,
                "jodo",
                "argument count mismatch",
                "use: jodo(\"file.txt\", \"data\")"
            );
            return value_null();
        }

        Value pathv = eval(node->args[0], env, rt);
        Value datav = eval(node->args[1], env, rt);

        if (pathv.type != VAL_STRING || !pathv.string) {
            value_free(&pathv);
            value_free(&datav);
            shriji_error(
                E_PARSE_02,
                "jodo",
                "file path must be string",
                "use: jodo(\"file.txt\", \"data\")"
            );
            return value_null();
        }

        const char *path = strip_quotes(pathv.string);

        const char *data = "";
        char tmp[128];
        tmp[0] = '\0';

        char *tmp_data = NULL;

        if (datav.type == VAL_STRING && datav.string) {
            tmp_data = unescape_string(strip_quotes(datav.string));
            data = tmp_data ? tmp_data : "";
        } else if (datav.type == VAL_NUMBER) {
            snprintf(tmp, sizeof(tmp), "%g", datav.number);
            data = tmp;
        }

        FILE *fp = fopen(path, "ab");
        if (!fp) {
            if (tmp_data) free(tmp_data);
            value_free(&pathv);
            value_free(&datav);
            return value_number(0);
        }

        fwrite(data, 1, strlen(data), fp);
        fclose(fp);

        if (tmp_data) free(tmp_data);

        value_free(&pathv);
        value_free(&datav);

        return value_number(1);
    }

    /*──────────────────────────────────────────────
      NORMAL FUNCTION CALL
    ──────────────────────────────────────────────*/
Value fnv = env_get(env, node->function_name);

if (fnv.type != VAL_FUNCTION) {
    shriji_error(
        E_PARSE_INVALID_TOKEN,
        "call",
        "function not found",
        "check name or define function"
    );
    return value_null();
}

ASTNode *fn = fnv.function;

    /* validate arg count */
    if (fn->rachna_param_count != node->arg_count) {
        shriji_error(
            E_PARSE_02,
            node->function_name,
            "argument count mismatch",
            "call with correct number of args"
        );
        return value_null();
    }

    /* save outer return state (nested calls safe) */
    int old_return_flag = rt->return_flag;
    Value old_return_value = rt->return_value;

    rt->return_flag = 0;
    rt->return_value = value_null();

    /* new call scope */
    env_push_scope(env);

    /* bind params */
       for (int i = 0; i < fn->rachna_param_count; i++) {
    ASTNode *p = fn->rachna_params[i];
        Value av = eval(node->args[i], env, rt);
        env_set(env, p->name, av);
    }

    /* run body */
    Value result = eval(fn->rachna_body, env, rt);

    if (rt->return_flag)
        result = rt->return_value;

    /* end call scope */
    env_pop_scope(env);

    /* restore outer return state */
    rt->return_flag = old_return_flag;
    rt->return_value = old_return_value;

   rt->call_depth--;
    return result;
}

/*──────────────────────────────────────────────
      CORE VALUES
    ──────────────────────────────────────────────*/
case AST_NUMBER:
    return value_number(node->number_value);

case AST_STRING:
    return value_string(node->string_value);

case AST_BOOL:
    return value_bool(node->bool_value);



    case AST_LIST: {
        int n = node->element_count;

        if (n <= 0 || !node->elements) {
            return value_list(NULL, 0);
        }

        Value *items = (Value *)malloc(sizeof(Value) * n);
        if (!items) return value_null();

        for (int i = 0; i < n; i++) {
            Value ev = eval(node->elements[i], env, rt);
            items[i] = value_copy(ev);
            value_free(&ev);
        }

        return value_list(items, n);
    }


case AST_DICT: {
        int n = node->dict_count;

        if (n <= 0 || !node->dict_keys || !node->dict_values) {
            return value_dict(NULL, NULL, 0);
        }

        Value *keys = (Value *)malloc(sizeof(Value) * n);
        Value *vals = (Value *)malloc(sizeof(Value) * n);

        if (!keys || !vals) {
            if (keys) free(keys);
            if (vals) free(vals);
            return value_null();
        }

        for (int i = 0; i < n; i++) {
            Value kv = eval(node->dict_keys[i], env, rt);
            Value vv = eval(node->dict_values[i], env, rt);

            keys[i] = value_copy(kv);
            vals[i] = value_copy(vv);

            value_free(&kv);
            value_free(&vv);
        }

        return value_dict(keys, vals, n);
    }

case AST_INDEX: {

    Value tv = eval(node->index_target, env, rt);
    Value iv = eval(node->index_expr, env, rt);

    /*──────────────────────────────────────────────
      LIST INDEXING: a[0]
    ──────────────────────────────────────────────*/
    if (tv.type == VAL_LIST) {

        if (!tv.items) {
            value_free(&tv);
            value_free(&iv);
            return value_null();
        }

        if (iv.type != VAL_NUMBER) {
            shriji_error(
                E_RUNTIME_01,
                "list",
                "List index number hona chahiye",
                "example: a[0]"
            );

               value_free(&tv);
               value_free(&iv);
               return value_null();
        }

        long long idx = iv.number;
        value_free(&iv);

        if (idx < 0 || idx >= tv.count) {
            shriji_error(
                E_RUNTIME_01,
                "list",
                "List index out of range",
                "example: a[0]"
            );
           value_free(&tv);
           return value_null();
        }

        Value out = value_copy(tv.items[(int)idx]);
        value_free(&tv);
        return out;
    }

    /*──────────────────────────────────────────────
      DICT INDEXING: d["a"]
    ──────────────────────────────────────────────*/
    if (tv.type == VAL_DICT) {

        if (iv.type != VAL_STRING || !iv.string) {
            shriji_error(
                E_RUNTIME_01,
                "dict",
                "dictionary ki key string honi chahiye",
                "example: d[\"a\"]"
            );
            value_free(&tv);
            value_free(&iv);
            return value_null();
        }

        for (int i = 0; i < tv.dict_count; i++) {

            Value k = tv.dict_keys[i];

            if (k.type == VAL_STRING &&
                strcmp(k.string, iv.string) == 0) {

                Value out = value_copy(tv.dict_values[i]);
                value_free(&tv);
                value_free(&iv);
                return out;
            }
        }

        shriji_error(
            E_RUNTIME_01,
            "dict",
            "dictionary me ye key maujood nahi hai",
            "example: d[\"a\"]"
        );
        value_free(&tv);
        value_free(&iv);
        return value_null();
    }

    /*──────────────────────────────────────────────
      UNSUPPORTED TARGET
    ──────────────────────────────────────────────*/
    value_free(&tv);
    value_free(&iv);
    return value_null();
}


case AST_INDEX_UPDATE: {

    /* target must be identifier so update persists */
    if (!node->index_target || node->index_target->type != AST_IDENTIFIER) {
        return value_null();
    }

    const char *var_name = node->index_target->name;

    /* fetch container directly from env */
    Value tv = env_get(env, var_name);

    Value iv = eval(node->index_expr, env, rt);
    Value vv = eval(node->index_value, env, rt);

    /*──────────────────────────────────────────────
      LIST INDEX UPDATE: a[0] = value
    ──────────────────────────────────────────────*/
    if (tv.type == VAL_LIST) {

        if (iv.type != VAL_NUMBER) goto cleanup;

        long long idx = iv.number;
        if (idx < 0 || idx >= tv.count) goto cleanup;

        value_free(&tv.items[(int)idx]);
        tv.items[(int)idx] = value_copy(vv);

        /* WRITE BACK */
        env_set(env, var_name, tv);

        value_free(&iv);
        return value_copy(vv);
    }

    /*──────────────────────────────────────────────
      DICT INDEX UPDATE: d["a"] = value
    ──────────────────────────────────────────────*/
    if (tv.type == VAL_DICT) {

        if (iv.type != VAL_STRING || !iv.string) goto cleanup;

        /* update if exists */
        for (int i = 0; i < tv.dict_count; i++) {
            Value k = tv.dict_keys[i];
            if (k.type == VAL_STRING && strcmp(k.string, iv.string) == 0) {

                value_free(&tv.dict_values[i]);
                tv.dict_values[i] = value_copy(vv);

                env_set(env, var_name, tv);
                value_free(&iv);
                return value_copy(vv);
            }
        }

        /* append new key */
        tv.dict_keys = realloc(tv.dict_keys,
                               sizeof(Value) * (tv.dict_count + 1));
        tv.dict_values = realloc(tv.dict_values,
                                 sizeof(Value) * (tv.dict_count + 1));

        tv.dict_keys[tv.dict_count]   = value_copy(iv);
        tv.dict_values[tv.dict_count] = value_copy(vv);
        tv.dict_count++;

        env_set(env, var_name, tv);
        value_free(&iv);
        return value_copy(vv);
    }

cleanup:
    value_free(&tv);
    value_free(&iv);
    value_free(&vv);
    return value_null();
}

case AST_IDENTIFIER: {

    const char *name = node->name;

    /* ===== ENV CHECK ===== */
    if (env_exists(env, name)) {
        return env_get(env, name);
    }

    /* ===== SESSION KEYWORD: last ===== */
    if (strcmp(name, "last") == 0) {
        return smriti_session_get_last();
    }

    /* ===== SMRITI FALLBACK ===== */
    const char *mem = smriti_personal_get(name);

    if (mem) {
        Value v;
        v.type = VAL_NUMBER;
        v.number = atof(mem);
        return v;
    }

    /* ===== ERROR ===== */
    shriji_error(
        E_ASSIGN_01,
        name,
        "variable declare nahi hua",
        "use mavi x = ... pehle likhiye"
    );

    return value_null();
}


case AST_ASSIGNMENT: {

    Value value = eval(node->value, env, rt);

    /* ===== FIX: SAFE COPY ===== */
    Value stored = value_copy(value);

    /* ===== ENV SAVE ===== */
    env_update(env, node->name, stored);

    /* ===== SMRITI SAVE ===== */
    if (value.type == VAL_NUMBER) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%g", value.number);

        if (node->name[0] != '\0') {
            /* smriti_personal_set(node->name, buf); */
        }
    }

    event_fire(EVENT_ASSIGNMENT, node->name);
    state_on_success(&rt->state);

    return value;
}



case AST_UPDATE: {

    if (node->name[0] != '\0') {

        if (!env_exists(env, node->name)) {
            shriji_error(
                E_ASSIGN_01,
                node->name,
                "variable not declared",
                "use mavi x = ... first"
            );
            return value_null();
        }

        Value value = eval(node->value, env, rt);

        /*  FIXED: use env_update */
        env_update(env, node->name, value);

        event_fire(EVENT_ASSIGNMENT, node->name);
        state_on_success(&rt->state);

        return value;
    }

    shriji_error(
        E_ASSIGN_01,
        "update",
        "invalid update target",
        "use: x = expr"
    );

    return value_null();
}




case AST_BINARY: {
    Value Lv = eval(node->left, env, rt);
    Value Rv = eval(node->right, env, rt);

    char op = node->op[0];

    /* ───────────────────────────────────────────────
       TYPE CHECK
    ─────────────────────────────────────────────── */
    if (Lv.type != Rv.type) {
        shriji_error(
            E_RUNTIME_TYPE_MISMATCH,
            "binary",
            "type mismatch in expression",
            "same type ke values use karein"
        );
        return value_null();
    }

    /* ───────────────────────────────────────────────
       NUMBER OPERATIONS
    ─────────────────────────────────────────────── */
/*  LOGICAL OPERATORS (GLOBAL — BEFORE TYPE CHECK) */
if (op == '&') {
    return value_bool(
        value_is_truthy(Lv) && value_is_truthy(Rv)
    );
}

if (op == '|') {
    return value_bool(
        value_is_truthy(Lv) || value_is_truthy(Rv)
    );
}


    if (Lv.type == VAL_NUMBER) {

        double L = Lv.number;
        double R = Rv.number;


        if (op == '+') return value_number(L + R);
        if (op == '-') return value_number(L - R);
        if (op == '*') return value_number(L * R);
        if (op == '/') return value_number(safe_div(L, R));
        if (op == '%') return value_number(safe_mod(L, R));

        if (op == '>') return value_bool(L > R);
        if (op == '<') return value_bool(L < R);
        if (op == 'G') return value_bool(L >= R);
        if (op == 'L') return value_bool(L <= R);
        if (op == '=') return value_bool(L == R);
        if (op == '!') return value_bool(L != R);

        shriji_error(
            E_RUNTIME_01,
            "binary",
            "unknown numeric operator",
            NULL
        );
        return value_null();
    }

    /* ───────────────────────────────────────────────
       STRING COMPARISON ONLY
    ─────────────────────────────────────────────── */
    if (Lv.type == VAL_STRING) {

        const char *L = Lv.string ? Lv.string : "";
        const char *R = Rv.string ? Rv.string : "";

        int cmp = strcmp(L, R);

         if (op == '=') return value_bool(cmp == 0);
         if (op == '!') return value_bool(cmp != 0);
         if (op == '>') return value_bool(cmp > 0);
         if (op == '<') return value_bool(cmp < 0);
         if (op == 'G') return value_bool(cmp >= 0);
         if (op == 'L') return value_bool(cmp <= 0);


        shriji_error(
            E_RUNTIME_01,
            "binary",
            "string ke saath sirf comparison allowed hai",
            "use: == != < > <= >="
        );
        return value_null();
    }

    /* ───────────────────────────────────────────────
       UNSUPPORTED TYPE
    ─────────────────────────────────────────────── */
    shriji_error(
        E_PARSE_02,
        "binary",
        "ye operation is type ke liye allowed nahi hai",
        "sirf number ya string comparison supported hai"
    );
    rt->state.safety = STATE_CRITICAL;
                return value_null();
}


       case AST_NOT: {
    Value v = eval(node->left, env, rt);

    int truth = value_is_truthy(v);
    value_free(&v);

    return value_bool(!truth);
    }


case AST_IF: {
    Value condv = eval(node->condition, env, rt);

    Value result = value_null();

    if (value_is_truthy(condv)) {

        env_push_scope(env);

        result = eval(node->left, env, rt);

        env_pop_scope(env);
    }
    else if (node->else_block) {

        env_push_scope(env);

        result = eval(node->else_block, env, rt);

        env_pop_scope(env);
    }

    value_free(&condv);
    return result;
}



case AST_WHILE: {

    rt->loop_depth++;

    int local_guard = 0;

    while (1) {

        Value condv = eval(node->condition, env, rt);

        if (!value_is_truthy(condv)) {
            value_free(&condv);
            break;
        }

        value_free(&condv);

        rt->continue_flag = 0;

        eval(node->body, env, rt);

        if (rt->return_flag) {
            rt->loop_depth--;
            return rt->return_value;
        }

        if (rt->break_flag) {
            rt->break_flag = 0;
            break;
        }

        if (rt->continue_flag) {
            rt->continue_flag = 0;
            continue;
        }

        if (++local_guard > 1000000) {
            shriji_error(
                E_RUNTIME_01,
                "loop",
                "infinite loop detected",
                "use rukja or fix condition"
            );
            rt->state.safety = STATE_CRITICAL;
            break;
        }
    }

    rt->loop_depth--;
    return value_null();
}



case AST_BLOCK: {
    Value last = value_null();


    /*  NEW SCOPE START */
    env_push_scope(env);


    for (int i = 0; i < node->stmt_count; i++) {

        last = eval(node->statements[i], env, rt);

        if (rt->return_flag || rt->break_flag || rt->continue_flag)
            break;
    }

    /*  SCOPE END */
    env_pop_scope(env);


    if (rt->return_flag)
        return rt->return_value;

    return last;
}


case AST_PROGRAM: {
    Value last = value_null();


    for (int i = 0; i < node->stmt_count; i++) {

        last = eval(node->statements[i], env ,rt);

/* 🔥 ONLY break for loop control, not function return */
if (rt->break_flag || rt->continue_flag)
    break;
    }

    if (rt->return_flag)
        return rt->return_value;

    return last;
}

case AST_IMPORT: {

    /* 🔥 ENABLE ANALYSIS MODE */
    shriji_set_error_mode(ERROR_MODE_COLLECT);

    /* import "file.sri" */
    const char *path = node->name;

    if (!path || !*path) {
        shriji_error(
            E_PARSE_02,
            "import",
            "empty import path",
            "use: import \"file.sri\""
        );
        shriji_set_error_mode(ERROR_MODE_IMMEDIATE);
        return value_null();
    }

    /* read file */
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        shriji_error(
            E_PARSE_02,
            "import",
            "file not found",
            "check path or create file first"
        );
        shriji_set_error_mode(ERROR_MODE_IMMEDIATE);
        return value_null();
    }

    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (sz < 0) sz = 0;
    if (sz > 1024 * 1024) sz = 1024 * 1024;

    char *buf = (char *)malloc((size_t)sz + 1);
    if (!buf) {
        fclose(fp);
        shriji_set_error_mode(ERROR_MODE_IMMEDIATE);
        return value_null();
    }

    size_t nread = fread(buf, 1, (size_t)sz, fp);
    buf[nread] = '\0';
    fclose(fp);

    /* 🔥 FULL FILE PARSE */
    ASTNode *prog = parse_program(buf);
    free(buf);

    if (!prog) {
        shriji_error(
            E_PARSE_02,
            "import",
            "parse failed",
            "check full file syntax"
        );

        shriji_print_all_errors();
        shriji_set_error_mode(ERROR_MODE_IMMEDIATE);
        return value_null();
    }

    /* 🔥 RUN FULL PROGRAM */
    Value result = eval(prog, env, rt);

    /* 🔥 PRINT ALL ERRORS (ONE SHOT) */
    shriji_print_all_errors();

    /* 🔥 BACK TO NORMAL MODE */
    shriji_set_error_mode(ERROR_MODE_IMMEDIATE);

    return result;
}

case AST_COMMAND: {

    CommandType cmd = resolve_command(node->command_name);
    if (cmd == CMD_UNKNOWN) {
        shriji_error(
            E_PARSE_02,
            "command",
            "unknown command",
            "command not recognized"
        );
        return value_null();
    }

if (cmd == CMD_BOLO) {

    /* 🔥 GLOBAL ERROR GUARD */
    if (error_reported) {
        return value_null();
    }

    Value v = value_null();

    if (node->value)
        v = eval(node->value, env , rt);

/*  RUNTIME ERROR GUARD */
if (rt->error_flag) {
    return value_null();
}

if (v.type == VAL_STRING) {
    printf("%s\n", v.string ? v.string : "");
}
else if (v.type == VAL_NUMBER) {
    printf("%g\n", v.number);
}
else if (v.type == VAL_BOOL) {
    printf("%d\n", v.boolean);
}
else if (v.type == VAL_LIST) {
    printf("[");
    for (int i = 0; i < v.count; i++) {
        if (i > 0) printf(", ");

        if (v.items[i].type == VAL_NUMBER)
            printf("%g", v.items[i].number);
        else if (v.items[i].type == VAL_STRING)
            printf("\"%s\"", v.items[i].string);
        else if (v.items[i].type == VAL_BOOL)
            printf("%d", v.items[i].boolean);
        else
            printf("?");
    }
    printf("]\n");
}
else {
    /*  DOUBLE GUARD */
    if (!rt->error_flag && !error_reported) {
        printf("0\n");
    }
}



    smriti_session_set_last(v);

    value_free(&v);
    return value_null();
}

/* AI MODULES → ai_router (STEP-13 REAL OUTPUT) */
if (cmd == CMD_SAKHI || cmd == CMD_NIYU ||
    cmd == CMD_MIRA  || cmd == CMD_KAVYA) {

    Value v = value_null();
    if (node->value)
        v = eval(node->value, env , rt);

    const char *arg = "";
    char tmp[64] = {0};

    if (v.type == VAL_STRING && v.string)
        arg = strip_quotes(v.string);
    else if (v.type == VAL_NUMBER) {
        snprintf(tmp, sizeof(tmp), "%g", v.number);
        arg = tmp;
    }

    ShrijiBridgePacket pkt;
    pkt.text   = arg;
    pkt.lang   = SHRIJI_LANG_AUTO;
    pkt.source = SHRIJI_SRC_REPL;

    L3Response r = ai_router_dispatch(&pkt);
    if (r.text && *r.text)
        printf("%s\n", r.text);

    value_free(&v);
    return value_null();
}

    /* DEFAULT COMMAND */
    return value_number(execute_command(cmd));
}

default:
    return value_null();
}
}

/*──────────────────────────────────────────────────────────────
 | PROGRAM RUNNER
 *──────────────────────────────────────────────────────────────*/
Value run_program(ASTNode *program, Env *env, ShrijiRuntime *rt)
{
    if (!program || program->type != AST_PROGRAM)
        return value_null();

    runtime_reset(rt);
    event_bind_runtime(rt);
    return eval(program, env, rt);
}
