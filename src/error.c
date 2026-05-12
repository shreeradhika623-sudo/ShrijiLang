#include <stdio.h>
#include <string.h>

#include "../include/error.h"
#include "../include/runtime.h"
#include "../gyaan/core/gyaan_engine.h"

/*──────────────────────────────────────────────*/
int error_reported = 0;
int GYAAN_ALREADY_PRINTED = 0;

int SHRIJI_ERROR_MODE = 0; 
// 0 = simple (default)
// 1 = full (deep insight)

static ShrijiErrorMode CURRENT_ERROR_MODE = ERROR_MODE_IMMEDIATE;

ShrijiErrorInfo LAST_ERROR = {0};

/*──────────────────────────────────────────────*/
/* ERROR STORAGE (KEEP — FUTURE MULTI ERROR ENGINE) */
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
/* 🌸 UI MAP (KEEP — USED BY KRST / FUTURE UI) */

typedef struct {
    ShrijiErrorCode code;
    const char *title;
    const char *message;
    const char *hint;
    const char *suggestion;
} ShrijiErrorUI;

static ShrijiErrorUI ERROR_UI_MAP[] = {

    {
        E_RUNTIME_DIV_ZERO,
        "Divide by Zero",
        "0 se divide nahi kar sakte",
        "denominator 0 nahi hona chahiye",
        "10 / 2 try karo"
    },

    {
        E_PARSE_OPERATOR_CHAIN,
        "Extra Operator",
        "Do operator ek saath aa gaye",
        "ek hi operator use karo",
        "6 + 6 likho"
    },

    {
        E_ASSIGN_01,
        "Unknown Variable",
        "Variable pehle declare nahi hua",
        "mavi x = 10 likho",
        "pehle variable define karo"
    }

};

static const ShrijiErrorUI* get_error_ui(ShrijiErrorCode code)
{
    int count = sizeof(ERROR_UI_MAP) / sizeof(ERROR_UI_MAP[0]);

    for (int i = 0; i < count; i++) {
        if (ERROR_UI_MAP[i].code == code)
            return &ERROR_UI_MAP[i];
    }

    return NULL;
}

/*──────────────────────────────────────────────*/
void shriji_set_error_mode(ShrijiErrorMode mode)
{
    CURRENT_ERROR_MODE = mode;
}

/*──────────────────────────────────────────────*/
/* STRING CODE (KEEP FOR DEBUG / LOGGING) */

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
/* AUTO MESSAGE */

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
/* STORE ERROR */

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
/* MAIN ERROR (NO PRINT — ONLY STORE + STATE) */

void shriji_error(
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
)
{
    if (error_reported)
        return;

    error_reported = 1;
    GYAAN_ALREADY_PRINTED = 0;

    if (current_runtime) {
        current_runtime->error_flag = 1;
    }

    const char *auto_msg = error_message_for_code(code);
    if (auto_msg)
        message = auto_msg;

LAST_ERROR.code = code;

snprintf(LAST_ERROR.context, sizeof(LAST_ERROR.context), "%s", context ? context : "");
snprintf(LAST_ERROR.message, sizeof(LAST_ERROR.message), "%s", message ? message : "");
snprintf(LAST_ERROR.hint, sizeof(LAST_ERROR.hint), "%s", hint ? hint : "");
snprintf(LAST_ERROR.function, sizeof(LAST_ERROR.function), "%s", "runtime");
    LAST_ERROR.has_location = 0;

    /* STORE IF COLLECT MODE */
    if (CURRENT_ERROR_MODE == ERROR_MODE_COLLECT) {
        store_error(code, context, message, hint, 0, 0);
    }
}

/*──────────────────────────────────────────────*/
/* ERROR WITH LOCATION */

void shriji_error_at_full(
    Token tok,
    ShrijiErrorCode code,
    const char *context,
    const char *function,
    const char *message,
    const char *hint
)
{
    if (error_reported)
        return;

    error_reported = 1;
    GYAAN_ALREADY_PRINTED = 0;

    if (current_runtime) {
        current_runtime->error_flag = 1;
    }

    const char *auto_msg = error_message_for_code(code);
    if (auto_msg)
        message = auto_msg;

    LAST_ERROR.code = code;

    snprintf(LAST_ERROR.context, sizeof(LAST_ERROR.context), "%s", context ? context : "");
    snprintf(LAST_ERROR.message, sizeof(LAST_ERROR.message), "%s", message ? message : "");
    snprintf(LAST_ERROR.hint, sizeof(LAST_ERROR.hint), "%s", hint ? hint : "");
    snprintf(LAST_ERROR.function, sizeof(LAST_ERROR.function), "%s", function ? function : "");

/*  NEW INTELLIGENCE FIELDS */
snprintf(LAST_ERROR.expected, sizeof(LAST_ERROR.expected), "%s", "unknown");
snprintf(LAST_ERROR.received, sizeof(LAST_ERROR.received), "%s", "unknown");

    LAST_ERROR.has_location = 1;
    LAST_ERROR.line = tok.line;
    LAST_ERROR.col = tok.col;

    if (CURRENT_ERROR_MODE == ERROR_MODE_COLLECT) {
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
/* PRINT ALL ERRORS (KEEP — USED IN IMPORT MODE) */

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
}

/*──────────────────────────────────────────────*/
const ShrijiErrorInfo *shriji_last_error(void)
{
    if (!error_reported)
        return NULL;

    return &LAST_ERROR;
}
