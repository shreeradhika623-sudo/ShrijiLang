#include <stdio.h>
#include <string.h>
#include "../include/error.h"

int error_reported = 0;

static ShrijiErrorMode CURRENT_ERROR_MODE = ERROR_MODE_IMMEDIATE;

static ShrijiErrorInfo LAST_ERROR = {0};


/*──────────────────────────────────────────────
 | ERROR MODE
 *──────────────────────────────────────────────*/
void shriji_set_error_mode(ShrijiErrorMode mode)
{
    CURRENT_ERROR_MODE = mode;
}


/*──────────────────────────────────────────────
 | ERROR CODE → STRING
 *──────────────────────────────────────────────*/
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


/*──────────────────────────────────────────────
 | AUTO MESSAGE SYSTEM
 *──────────────────────────────────────────────*/
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


/*──────────────────────────────────────────────
 | ERROR EMIT
 *──────────────────────────────────────────────*/
void shriji_error(
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
)
{
    error_reported = 1;

    const char *auto_msg = error_message_for_code(code);
    if (auto_msg)
        message = auto_msg;

    LAST_ERROR.code = code;
    LAST_ERROR.context = context;
    LAST_ERROR.message = message;
    LAST_ERROR.hint = hint;
    LAST_ERROR.has_location = 0;
    LAST_ERROR.line = 0;
    LAST_ERROR.col = 0;

    if (CURRENT_ERROR_MODE == ERROR_MODE_IMMEDIATE) {

        printf("\n⛔ %s | %s → %s\n",
               error_code_str(code),
               context ? context : "",
               message ? message : "");

        if (hint && hint[0])
            printf("   hint: %s\n", hint);

        printf("\n");
    }
}


/*──────────────────────────────────────────────
 | ERROR WITH LOCATION
 *──────────────────────────────────────────────*/
void shriji_error_at(
    Token tok,
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
)
{
    error_reported = 1;

    const char *auto_msg = error_message_for_code(code);
    if (auto_msg)
        message = auto_msg;

    LAST_ERROR.code = code;
    LAST_ERROR.context = context;
    LAST_ERROR.message = message;
    LAST_ERROR.hint = hint;
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
    }
}


/*──────────────────────────────────────────────
 | LAST ERROR
 *──────────────────────────────────────────────*/
const ShrijiErrorInfo *shriji_last_error(void)
{
    if (!error_reported)
        return NULL;

    return &LAST_ERROR;
}


/*──────────────────────────────────────────────
 | GUIDE
 *──────────────────────────────────────────────*/
void shriji_print_guide(void)
{
    const ShrijiErrorInfo *e = shriji_last_error();

    if (!e) {
        printf("\nKoi error pending nahi hai.\n\n");
        return;
    }

    printf("\nError: %s\n", e->message ? e->message : "");

    if (e->hint && e->hint[0])
        printf("Hint: %s\n", e->hint);

    printf("\n");
}
