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
 | GLOBAL EXECUTION STATE
 *──────────────────────────────────────────────────────────────*/
ExecutionState SHRIJI_STATE;

/* Loop control flags */
static int BREAK_FLAG = 0;
static int CONTINUE_FLAG = 0;
static int LOOP_DEPTH = 0;

/* Return control flags */
static int RETURN_FLAG = 0;
static Value RETURN_VALUE;

/*──────────────────────────────────────────────────────────────
 | STATE DECISION HOOK
 *──────────────────────────────────────────────────────────────*/
static int state_allows_execution(void)
{
    if (SHRIJI_STATE.safety == STATE_CRITICAL) {
        /* execution halted due to critical state */
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
            E_PARSE_02,
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
            E_PARSE_02,
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
static double value_to_number(Value v)
{
    if (v.type == VAL_NUMBER)
        return v.number;
    return 0;
}

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
Value eval(ASTNode *node, Env *env)
{
    static int state_initialized = 0;

    if (!state_initialized) {
        state_init(&SHRIJI_STATE);
        state_initialized = 1;
    }

    if (!state_allows_execution() || !node)
        return value_null();

    /* 🧠 GLOBAL STEP COUNTER */
    if (node->type != AST_WHILE &&
        node->type != AST_BREAK &&
        node->type != AST_CONTINUE &&
        node->type != AST_BLOCK &&
        node->type != AST_PROGRAM) {
        SHRIJI_STATE.steps_used++;
    }

    if (SHRIJI_STATE.steps_used > SHRIJI_STATE.max_steps) {
        shriji_error(
            E_PARSE_02,
            "runtime",
            "execution limit exceeded",
            "possible infinite loop detected"
        );
        SHRIJI_STATE.safety = STATE_CRITICAL;
        return value_null();
    }



switch (node->type) {



case AST_BREAK:

    if (LOOP_DEPTH <= 0) {
        shriji_error(
        E_RUNTIME_01,
        "break",
        "loop ke bahar rukja allowed nahi hai",
        "rukja sirf while loop ke andar use karein"
    );
    return value_null();
}


   BREAK_FLAG = 1;
    return value_null();


case AST_CONTINUE:

     if (LOOP_DEPTH <= 0) {
        shriji_error(
        E_RUNTIME_01,
        "continue",
        "loop ke bahar continue allowed nahi hai",
        "continue sirf while loop ke andar use karein"
    );
    return value_null();
}
    CONTINUE_FLAG = 1;
    return value_null();






case AST_RETURN: {
        Value v = value_null();
        if (node->return_expr)
            v = eval(node->return_expr, env);

        RETURN_FLAG = 1;
        RETURN_VALUE = v;
        return v;
    }




/*────────────────────────────────────────
 | INC / DEC (++ / --)
 *────────────────────────────────────────*/
case AST_INC:
case AST_DEC: {

    /* only identifiers allowed */
    if (!node->left || node->left->type != AST_IDENTIFIER) {
        return value_null();
    }

    const char *name = node->left->name;

    /* read current value */
    Value old = env_get(env, name);

    if (old.type != VAL_NUMBER) {
        return value_null();
    }

    /* compute new value */
    Value newv = old;

    if (node->type == AST_INC)
        newv.number = old.number + 1;
    else
        newv.number = old.number - 1;

    /* write back */
    env_set(env, name, newv);

    /* prefix vs postfix */
    if (node->is_prefix)
        return newv;   /* ++x / --x */
    else
        return old;    /* x++ / x-- */
}



    /*──────────────────────────────────────────────
      FUNCTIONS
    ──────────────────────────────────────────────*/


    case AST_FUNCTION: {
        /* store function AST node into env */
        env_set(env, node->function_name, value_function(node));
        return value_null();
    }


case AST_CALL: {

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

        Value pathv = eval(node->args[0], env);

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

        Value pathv = eval(node->args[0], env);
        Value datav = eval(node->args[1], env);

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

        Value pathv = eval(node->args[0], env);
        Value datav = eval(node->args[1], env);

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

    /* find function */
    if (!env_exists(env, node->function_name)) {
        shriji_error(
            E_PARSE_02,
            node->function_name,
            "function not found",
            "define using: kaam name(...) { ... }"
        );
        return value_null();
    }

    Value fnv = env_get(env, node->function_name);
    if (fnv.type != VAL_FUNCTION || !fnv.function) {
        shriji_error(
            E_PARSE_02,
            node->function_name,
            "not a function",
            "name must refer to a function"
        );
        return value_null();
    }

    ASTNode *fn = fnv.function;

    /* validate arg count */
    if (fn->param_count != node->arg_count) {
        shriji_error(
            E_PARSE_02,
            node->function_name,
            "argument count mismatch",
            "call with correct number of args"
        );
        return value_null();
    }

    /* save outer return state (nested calls safe) */
    int old_return_flag = RETURN_FLAG;
    Value old_return_value = RETURN_VALUE;

    RETURN_FLAG = 0;
    RETURN_VALUE = value_null();

    /* new call scope */
    env_push_scope(env);

    /* bind params */
    for (int i = 0; i < fn->param_count; i++) {
        ASTNode *p = fn->params[i];
        Value av = eval(node->args[i], env);
        env_set(env, p->name, av);
    }

    /* run body */
    Value result = eval(fn->body, env);

    if (RETURN_FLAG)
        result = RETURN_VALUE;

    /* end call scope */
    env_pop_scope(env);

    /* restore outer return state */
    RETURN_FLAG = old_return_flag;
    RETURN_VALUE = old_return_value;

    return result;
}

/*──────────────────────────────────────────────
      CORE VALUES
    ──────────────────────────────────────────────*/
    case AST_NUMBER:
        return value_number(node->number_value);

    case AST_STRING:
        return value_string(node->string_value);

    case AST_LIST: {
        int n = node->element_count;

        if (n <= 0 || !node->elements) {
            return value_list(NULL, 0);
        }

        Value *items = (Value *)malloc(sizeof(Value) * n);
        if (!items) return value_null();

        for (int i = 0; i < n; i++) {
            Value ev = eval(node->elements[i], env);
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
            Value kv = eval(node->dict_keys[i], env);
            Value vv = eval(node->dict_values[i], env);

            keys[i] = value_copy(kv);
            vals[i] = value_copy(vv);

            value_free(&kv);
            value_free(&vv);
        }

        return value_dict(keys, vals, n);
    }

case AST_INDEX: {

    Value tv = eval(node->index_target, env);
    Value iv = eval(node->index_expr, env);

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

    Value iv = eval(node->index_expr, env);
    Value vv = eval(node->index_value, env);

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

    if (!env_exists(env, node->name)) {

        shriji_error(
            E_ASSIGN_01,
            node->name,
            "variable declare nahi hua",
            "use mavi x = ... pehle likhiye"
        );

        return value_null();   // ✅ soft failure
    }

    return env_get(env, node->name);
}

 case AST_ASSIGNMENT: {
        Value value = eval(node->value, env);
        env_set(env, node->name, value);

        event_fire(EVENT_ASSIGNMENT, node->name);
        state_on_success(&SHRIJI_STATE);

        return value;
    }

case AST_UPDATE: {

    /* normal variable update: x = expr */
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

        Value value = eval(node->value, env);
        env_update(env, node->name, value);

        event_fire(EVENT_ASSIGNMENT, node->name);
        state_on_success(&SHRIJI_STATE);

        return value;
    }

    /* future: dict/list update will come here */
    shriji_error(
        E_ASSIGN_01,
        "update",
        "invalid update target",
        "use: x = expr"
    );
    return value_null();
}



case AST_BINARY: {
    Value Lv = eval(node->left, env);
    Value Rv = eval(node->right, env);

    char op = node->op[0];

    /* ───────────────────────────────────────────────
       TYPE CHECK
    ─────────────────────────────────────────────── */
    if (Lv.type != Rv.type) {
        shriji_error(
            E_PARSE_02,
            "binary",
            "type mismatch in expression",
            "same type ke values use karein"
        );
        return value_null();
    }

    /* ───────────────────────────────────────────────
       NUMBER OPERATIONS
    ─────────────────────────────────────────────── */
    if (Lv.type == VAL_NUMBER) {

        double L = Lv.number;
        double R = Rv.number;


        if (op == '+') return value_number(L + R);
        if (op == '-') return value_number(L - R);
        if (op == '*') return value_number(L * R);
        if (op == '/') return value_number(safe_div(L, R));
        if (op == '%') return value_number(safe_mod(L, R));

        if (op == '>') return value_number(L > R);
        if (op == '<') return value_number(L < R);
        if (op == 'G') return value_number(L >= R); /* >= */
        if (op == 'L') return value_number(L <= R); /* <= */
        if (op == '=') return value_number(L == R); /* == */
        if (op == '!') return value_number(L != R); /* != */

        shriji_error(
            E_PARSE_02,
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

        if (op == '=') return value_number(cmp == 0); /* == */
        if (op == '!') return value_number(cmp != 0); /* != */
        if (op == '>') return value_number(cmp > 0);
        if (op == '<') return value_number(cmp < 0);
        if (op == 'G') return value_number(cmp >= 0); /* >= */
        if (op == 'L') return value_number(cmp <= 0); /* <= */

        shriji_error(
            E_PARSE_02,
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
    SHRIJI_STATE.safety = STATE_CRITICAL;
                return value_null();
}


    case AST_NOT: {
        Value v = eval(node->left, env);
        return value_number(!value_to_number(v));
    }

    case AST_IF: {
        Value condv = eval(node->condition, env);
        double cond = value_to_number(condv);

        if (cond)
            return eval(node->left, env);
        else if (node->else_block)
            return eval(node->else_block, env);

        return value_null();
    }



case AST_WHILE: {

    LOOP_DEPTH++;

    Value last = value_null();
    int local_guard = 0;

    while (value_to_number(eval(node->condition, env))) {

        CONTINUE_FLAG = 0;

        last = eval(node->body, env);

        /* return must exit loop immediately */
        if (RETURN_FLAG) {
            LOOP_DEPTH--;
            return RETURN_VALUE;
        }

        if (CONTINUE_FLAG) {
            CONTINUE_FLAG = 0;
            continue;
        }

        if (BREAK_FLAG) {
            BREAK_FLAG = 0;
            break;
        }

        if (++local_guard > 1000000) {
            shriji_error(
                E_PARSE_02,
                "loop",
                "infinite loop detected",
                "use rukja or fix condition"
            );
            SHRIJI_STATE.safety = STATE_CRITICAL;
            break;
        }
    }

    LOOP_DEPTH--;
    return last;
}




case AST_BLOCK: {
    Value last = value_null();

    /* ✅ new scope for { } */
    env_push_scope(env);

    for (int i = 0; i < node->stmt_count; i++) {

        last = eval(node->statements[i], env);

        if (RETURN_FLAG || BREAK_FLAG || CONTINUE_FLAG)
            break;
    }

    /* ✅ scope ends */
    env_pop_scope(env);

    if (RETURN_FLAG)
        return RETURN_VALUE;

    return last;
}

case AST_PROGRAM: {
    Value last = value_null();


  /*──────────────────────────────────────────────
  OPTION A — HUMAN SENTENCE INTERCEPT (L3)
──────────────────────────────────────────────*/
int all_identifiers = 1;
for (int i = 0; i < node->stmt_count; i++) {
    if (!node->statements[i] ||
        node->statements[i]->type != AST_IDENTIFIER) {
        all_identifiers = 0;
        break;
    }
}

if (all_identifiers && node->stmt_count > 0) {
    char msg[512] = {0};
    size_t pos = 0;

    for (int i = 0; i < node->stmt_count; i++) {
        const char *w = node->statements[i]->name;
        if (!w) continue;

        size_t len = strlen(w);
        if (pos + len + 2 >= sizeof(msg))
            break;

        memcpy(msg + pos, w, len);
        pos += len;
        msg[pos++] = ' ';
    }

    msg[pos ? pos - 1 : 0] = '\0';

    ai_router_process(msg);
    return value_null();
}



    for (int i = 0; i < node->stmt_count; i++) {

        last = eval(node->statements[i], env);

        if (RETURN_FLAG || BREAK_FLAG || CONTINUE_FLAG)
            break;
    }

    if (RETURN_FLAG)
        return RETURN_VALUE;

    return last;
}

case AST_IMPORT: {

        /* import "file.sri" */
        const char *path = node->name;

        if (!path || !*path) {
            shriji_error(
                E_PARSE_02,
                "import",
                "empty import path",
                "use: import \"file.sri\""
            );
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
            return value_null();
        }

        fseek(fp, 0, SEEK_END);
        long sz = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        if (sz < 0) sz = 0;
        if (sz > 1024 * 1024) sz = 1024 * 1024; /* 1MB limit */

        char *buf = (char *)malloc((size_t)sz + 1);
        if (!buf) {
            fclose(fp);
            return value_null();
        }

        size_t nread = fread(buf, 1, (size_t)sz, fp);
        buf[nread] = '\0';
        fclose(fp);

        /* parse imported program */
        ASTNode *prog = parse_program(buf);
        free(buf);

        if (!prog) {
            shriji_error(
                E_PARSE_02,
                "import",
                "parse failed",
                "check imported file syntax"
            );
            return value_null();
        }

        /* run imported program in SAME env */
        Value out = eval(prog, env);
        return out;
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

    /* BOLO */
    if (cmd == CMD_BOLO) {
        Value v = value_null();
        if (node->value)
            v = eval(node->value, env);

        if (v.type == VAL_STRING)
            printf("%s\n", v.string ? v.string : "");
        else if (v.type == VAL_NUMBER)
            printf("%g\n", v.number);
        else
            printf("0\n");

        value_free(&v);
        return value_null();
    }

    /* AI MODULES → ai_router */
    if (cmd == CMD_SAKHI || cmd == CMD_NIYU ||
        cmd == CMD_MIRA  || cmd == CMD_KAVYA) {

        Value v = value_null();
        if (node->value)
            v = eval(node->value, env);

        const char *arg = "";
        char tmp[64] = {0};

        if (v.type == VAL_STRING && v.string)
            arg = strip_quotes(v.string);
        else if (v.type == VAL_NUMBER) {
            snprintf(tmp, sizeof(tmp), "%g", v.number);
            arg = tmp;
        }

        char msg[256];
        snprintf(msg, sizeof(msg), "%s %s",
                 node->command_name, arg);

        ai_router_process(msg);
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
Value run_program(ASTNode *program, Env *env)
{
    BREAK_FLAG = 0;
    CONTINUE_FLAG = 0;
    RETURN_FLAG = 0;
    RETURN_VALUE = value_null();

    if (!program || program->type != AST_PROGRAM)
        return value_null();

    return eval(program, env);
}
