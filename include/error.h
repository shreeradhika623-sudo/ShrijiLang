#ifndef ERROR_H
#define ERROR_H

#include "../include/token.h"

typedef enum {

    E_UNKNOWN = 0,

    E_ASSIGN_01,
    E_ASSIGN_02,

    E_PARSE_01,
    E_PARSE_02,

    E_PARSE_EXTRA_OPERATOR,
    E_PARSE_MISSING_OPERATOR,
    E_PARSE_DOUBLE_OPERATOR,
    E_PARSE_OPERATOR_START,
    E_PARSE_OPERATOR_END,
    E_PARSE_OPERATOR_CHAIN,
    E_PARSE_MISSING_OPERAND,

    E_PARSE_BRACKET_MISSING,
    E_PARSE_BRACKET_EXTRA,
    E_PARSE_UNMATCHED_PAREN,
    E_PARSE_UNMATCHED_BRACKET,
    E_PARSE_UNMATCHED_BRACE,

    E_PARSE_INVALID_TOKEN,
    E_PARSE_EMPTY_EXPRESSION,

    E_FUNCTION_NAME_INVALID,
    E_FUNCTION_PARAM_INVALID,

    E_IMPORT_PATH_INVALID,

    E_LIST_SYNTAX_ERROR,
    E_DICT_KEY_INVALID,
    E_DICT_SYNTAX_ERROR,

    E_IF_01,

    E_RUNTIME_01,
    E_RUNTIME_DIV_ZERO,
    E_RUNTIME_TYPE_MISMATCH,
    E_RUNTIME_LOOP_LIMIT,
    E_RUNTIME_INDEX_ERROR

} ShrijiErrorCode;

typedef struct {

    ShrijiErrorCode code;

    const char *context;
    const char *message;
    const char *hint;

    int has_location;
    int line;
    int col;

} ShrijiErrorInfo;

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

const ShrijiErrorInfo *shriji_last_error(void);

extern int error_reported;

typedef enum {
    ERROR_MODE_IMMEDIATE,
    ERROR_MODE_SILENT,
    ERROR_MODE_COLLECT
} ShrijiErrorMode;

void shriji_set_error_mode(ShrijiErrorMode mode);

void shriji_print_guide(void);

int suggest_basic_fixes(const char *input, int pos, char *out_expr);

void shriji_print_all_errors(void);

#endif
