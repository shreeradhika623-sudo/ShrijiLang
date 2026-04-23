#include <stdio.h>
#include <string.h>

#include "../include/error.h"
#include "../include/runtime.h"
#include "../gyaan/core/gyaan_engine.h"

/*──────────────────────────────────────────────*/
int error_reported = 0;

static ShrijiErrorMode CURRENT_ERROR_MODE = ERROR_MODE_IMMEDIATE;

ShrijiErrorInfo LAST_ERROR = {0};


/* 🔥 ERROR STORAGE */
#define MAX_ERRORS 100

typedef struct {
    int line;
    int col;
    char code[32];
    char context[64];
    char message[256];
    char hint[256];
} StoredError;

static StoredError ERROR_LIST[MAX_ERRORS];
static int ERROR_COUNT = 0;


/*──────────────────────────────────────────────*/
void shriji_set_error_mode(ShrijiErrorMode mode)
{
    CURRENT_ERROR_MODE = mode;
}


/*──────────────────────────────────────────────*/
static const char *error_code_str(ShrijiErrorCode code)
{
    switch (code) {

        case E_ASSIGN_01: return "E_ASSIGN_01";
        case E_ASSIGN_02: return "E_ASSIGN_02";

        case E_PARSE_01: return "E_PARSE_01";
        case E_PARSE_02: return "E_PARSE_02";
        case E_PARSE_EXTRA_OPERATOR: return "E_PARSE_EXTRA_OPERATOR";
        case E_PARSE_MISSING_OPERATOR: return "E_PARSE_MISSING_OPERATOR";
        case E_PARSE_DOUBLE_OPERATOR: return "E_PARSE_DOUBLE_OPERATOR";
        case E_PARSE_OPERATOR_START: return "E_PARSE_OPERATOR_START";
        case E_PARSE_OPERATOR_END: return "E_PARSE_OPERATOR_END";
        case E_PARSE_OPERATOR_CHAIN: return "E_PARSE_OPERATOR_CHAIN";
        case E_PARSE_MISSING_OPERAND: return "E_PARSE_MISSING_OPERAND";

        case E_PARSE_UNMATCHED_PAREN: return "E_PARSE_UNMATCHED_PAREN";
        case E_PARSE_BRACKET_MISSING: return "E_PARSE_BRACKET_MISSING";
        case E_PARSE_BRACKET_EXTRA: return "E_PARSE_BRACKET_EXTRA";
        case E_PARSE_INVALID_TOKEN: return "E_PARSE_INVALID_TOKEN";

        case E_IF_01: return "E_IF_01";

        case E_RUNTIME_01: return "E_RUNTIME_01";
        case E_RUNTIME_DIV_ZERO: return "E_RUNTIME_DIV_ZERO";
        case E_RUNTIME_TYPE_MISMATCH: return "E_RUNTIME_TYPE_MISMATCH";
        case E_RUNTIME_LOOP_LIMIT: return "E_RUNTIME_LOOP_LIMIT";
        case E_RUNTIME_INDEX_ERROR: return "E_RUNTIME_INDEX_ERROR";

        default: return "E_UNKNOWN";
    }
}


/*──────────────────────────────────────────────*/
static const char *error_message_for_code(ShrijiErrorCode code)
{
    switch (code) {

        case E_PARSE_DOUBLE_OPERATOR:
            return "Do operators ek saath aa gaye hain.";

        case E_PARSE_OPERATOR_CHAIN:
            return "Operator sequence galat lag raha hai.";

        case E_PARSE_OPERATOR_START:
            return "Expression operator se start nahi ho sakta.";

        case E_PARSE_OPERATOR_END:
            return "Expression operator par end ho gaya.";

        case E_PARSE_MISSING_OPERAND:
            return "Operator ke liye value missing hai.";

        default:
            return NULL;
    }
}


/*──────────────────────────────────────────────*/
static void store_error(
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint,
    int line,
    int col
)
{
    if (ERROR_COUNT >= MAX_ERRORS) return;

    StoredError *e = &ERROR_LIST[ERROR_COUNT++];

    snprintf(e->code, sizeof(e->code), "%s", error_code_str(code));
    snprintf(e->context, sizeof(e->context), "%s", context ? context : "");
    snprintf(e->message, sizeof(e->message), "%s", message ? message : "");
    snprintf(e->hint, sizeof(e->hint), "%s", hint ? hint : "");

    e->line = line;
    e->col = col;
}


/*──────────────────────────────────────────────*/
void shriji_error(
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
)
{
    error_reported = 1;

    if (current_runtime) {
        current_runtime->error_flag = 1;
    }

    const char *auto_msg = error_message_for_code(code);
    if (auto_msg)
        message = auto_msg;

    LAST_ERROR.code = code;
    LAST_ERROR.context = context;
    LAST_ERROR.message = message;
    LAST_ERROR.hint = hint;
    LAST_ERROR.function = NULL;
    LAST_ERROR.has_location = 0;

    if (CURRENT_ERROR_MODE == ERROR_MODE_IMMEDIATE) {

        printf("\n⛔ %s | %s → %s\n",
               error_code_str(code),
               context ? context : "",
               message ? message : "");

        if (hint && hint[0])
            printf("   hint: %s\n", hint);

        printf("\n");

        const ShrijiErrorInfo *last = shriji_last_error();
        if (last) {
            gyaan_print(last);
        }

    } else {
        store_error(code, context, message, hint, 0, 0);
    }
}


/*──────────────────────────────────────────────*/
void shriji_error_at_full(
    Token tok,
    ShrijiErrorCode code,
    const char *context,
    const char *function,
    const char *message,
    const char *hint
)
{
    error_reported = 1;

    if (current_runtime) {
        current_runtime->error_flag = 1;
    }

    const char *auto_msg = error_message_for_code(code);
    if (auto_msg)
        message = auto_msg;

    LAST_ERROR.code = code;
    LAST_ERROR.context = context;
    LAST_ERROR.message = message;
    LAST_ERROR.hint = hint;
    LAST_ERROR.function = function;
    LAST_ERROR.has_location = 1;
    LAST_ERROR.line = tok.line;
    LAST_ERROR.col = tok.col;

    if (CURRENT_ERROR_MODE == ERROR_MODE_IMMEDIATE) {

        printf("\n⛔ %s | %s → %s (line %d, col %d)\n",
               error_code_str(code),
               context ? context : "",
               message ? message : "",
               tok.line,
               tok.col);

        if (hint && hint[0])
            printf("   hint: %s\n", hint);

        printf("\n");

        const ShrijiErrorInfo *last = shriji_last_error();
        if (last) {
            gyaan_print(last);
        }

    } else {
        store_error(code, context, message, hint, tok.line, tok.col);
    }
}


/*──────────────────────────────────────────────*/
void shriji_error_at(
    Token tok,
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
)
{
    shriji_error_at_full(
        tok,
        code,
        context,
        "unknown",
        message,
        hint
    );
}


/*──────────────────────────────────────────────*/
void shriji_print_all_errors(void)
{
    if (ERROR_COUNT == 0) return;

    printf("\nKRST ANALYSIS REPORT\n");
    printf("──────────────────────────────\n");

    for (int i = 0; i < ERROR_COUNT; i++) {
        StoredError *e = &ERROR_LIST[i];

        printf("%d) %s | %s → %s",
               i + 1,
               e->code,
               e->context,
               e->message);

        if (e->line > 0)
            printf(" (line %d, col %d)", e->line, e->col);

        printf("\n");

        if (e->hint[0])
            printf("   hint: %s\n", e->hint);

        printf("\n");
    }

    printf("──────────────────────────────\n\n");

    ERROR_COUNT = 0;

    const ShrijiErrorInfo *last = shriji_last_error();
    if (last) {
        gyaan_print(last);
    }
}


/*──────────────────────────────────────────────*/
const ShrijiErrorInfo *shriji_last_error(void)
{
    if (!error_reported)
        return NULL;

    return &LAST_ERROR;
}
