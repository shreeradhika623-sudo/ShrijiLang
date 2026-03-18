#ifndef ERROR_H
#define ERROR_H

#include "../include/token.h"

/* =========================================================
   SHRIJILANG ERROR SYSTEM
   Centralized error taxonomy
   ========================================================= */

typedef enum {

    /* -----------------------------------------------------
       UNKNOWN
    ----------------------------------------------------- */
    E_UNKNOWN = 0,


    /* -----------------------------------------------------
       ASSIGNMENT
    ----------------------------------------------------- */
    E_ASSIGN_01,
    E_ASSIGN_02,


    /* -----------------------------------------------------
       PARSE GENERIC
    ----------------------------------------------------- */
    E_PARSE_01,
    E_PARSE_02,


    /* -----------------------------------------------------
       OPERATOR STRUCTURE
    ----------------------------------------------------- */
    E_PARSE_EXTRA_OPERATOR,
    E_PARSE_MISSING_OPERATOR,
    E_PARSE_DOUBLE_OPERATOR,
    E_PARSE_OPERATOR_START,
    E_PARSE_OPERATOR_END,
    E_PARSE_OPERATOR_CHAIN,
    E_PARSE_MISSING_OPERAND,


    /* -----------------------------------------------------
       BRACKET ERRORS
    ----------------------------------------------------- */
    E_PARSE_BRACKET_MISSING,
    E_PARSE_BRACKET_EXTRA,
    E_PARSE_UNMATCHED_PAREN,
    E_PARSE_UNMATCHED_BRACKET,
    E_PARSE_UNMATCHED_BRACE,

    /* -----------------------------------------------------
       EXPRESSION STRUCTURE
    ----------------------------------------------------- */
    E_PARSE_INVALID_TOKEN,
    E_PARSE_EMPTY_EXPRESSION,


    /* -----------------------------------------------------
       FUNCTION ERRORS
    ----------------------------------------------------- */
    E_FUNCTION_NAME_INVALID,
    E_FUNCTION_PARAM_INVALID,


    /* -----------------------------------------------------
       IMPORT ERRORS
    ----------------------------------------------------- */
    E_IMPORT_PATH_INVALID,


    /* -----------------------------------------------------
       LIST / DICT ERRORS
    ----------------------------------------------------- */
    E_LIST_SYNTAX_ERROR,
    E_DICT_KEY_INVALID,
    E_DICT_SYNTAX_ERROR,


    /* -----------------------------------------------------
       CONTROL FLOW
    ----------------------------------------------------- */
    E_IF_01,


    /* -----------------------------------------------------
       RUNTIME
    ----------------------------------------------------- */
    E_RUNTIME_01,
    E_RUNTIME_DIV_ZERO,
    E_RUNTIME_TYPE_MISMATCH,
    E_RUNTIME_LOOP_LIMIT,
    E_RUNTIME_INDEX_ERROR

} ShrijiErrorCode;


/* =========================================================
   ERROR SNAPSHOT
   ========================================================= */

typedef struct {

    ShrijiErrorCode code;

    const char *context;
    const char *message;
    const char *hint;

    int has_location;
    int line;
    int col;

} ShrijiErrorInfo;


/* =========================================================
   ERROR EMITTER
   ========================================================= */

void shriji_error(
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
);

void shriji_error_at(
    Token tok,
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
);


/* =========================================================
   LAST ERROR
   ========================================================= */

const ShrijiErrorInfo *shriji_last_error(void);


/* =========================================================
   GLOBAL FLAG
   ========================================================= */

extern int error_reported;


/* =========================================================
   ERROR MODE
   ========================================================= */

typedef enum {
    ERROR_MODE_IMMEDIATE,
    ERROR_MODE_SILENT
} ShrijiErrorMode;

void shriji_set_error_mode(ShrijiErrorMode mode);


/* =========================================================
   GUIDE
   ========================================================= */

void shriji_print_guide(void);

int suggest_basic_fixes(const char *input, int pos, char *out_expr);

#endif
