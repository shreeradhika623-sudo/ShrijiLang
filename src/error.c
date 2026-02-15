#include <stdio.h>
#include "../include/error.h"

/*──────────────────────────────────────────────
  GLOBAL ERROR STATE
──────────────────────────────────────────────*/
int error_reported = 0;

/*──────────────────────────────────────────────
  LAST ERROR SNAPSHOT (L3)
──────────────────────────────────────────────*/
static ShrijiErrorInfo LAST_ERROR = {0};

/*──────────────────────────────────────────────
  INTERNAL: error code → string
──────────────────────────────────────────────*/
static const char *error_code_str(ShrijiErrorCode code)
{
    switch (code) {
        case E_ASSIGN_01: return "E_ASSIGN_01";
        case E_PARSE_01:  return "E_PARSE_01";
        case E_PARSE_02:  return "E_PARSE_02";
        case E_IF_01:     return "E_IF_01";
        case E_RUNTIME_01:return "E_RUNTIME_01";
        default:          return "E_UNKNOWN";
    }
}

/*──────────────────────────────────────────────
  ERROR WITHOUT LOCATION
──────────────────────────────────────────────*/
void shriji_error(
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
)
{
    error_reported = 1;

    /* store snapshot */
    LAST_ERROR.code = code;
    LAST_ERROR.context = context;
    LAST_ERROR.message = message;
    LAST_ERROR.hint = hint;
    LAST_ERROR.has_location = 0;
    LAST_ERROR.line = 0;
    LAST_ERROR.col = 0;

    /* print */
    printf("\n⛔ %s | %s → %s\n",
           error_code_str(code),
           context,
           message);

    if (hint && hint[0] != '\0') {
        printf("   hint: %s\n", hint);
    }

    printf("\n");
}

/*──────────────────────────────────────────────
  ERROR WITH TOKEN LOCATION
──────────────────────────────────────────────*/
void shriji_error_at(
    Token tok,
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
)
{
    error_reported = 1;

    /* store snapshot */
    LAST_ERROR.code = code;
    LAST_ERROR.context = context;
    LAST_ERROR.message = message;
    LAST_ERROR.hint = hint;
    LAST_ERROR.has_location = 1;
    LAST_ERROR.line = tok.line;
    LAST_ERROR.col = tok.col;

    /* print */
    printf("\n⛔ %s | %s → %s (line %d, col %d)\n",
           error_code_str(code),
           context,
           message,
           tok.line,
           tok.col);

    if (tok.start && tok.length > 0) {
        printf("   token: '%.*s'\n", tok.length, tok.start);
    }

    if (hint && hint[0] != '\0') {
        printf("   hint: %s\n", hint);
    }

    printf("\n");
}

/*──────────────────────────────────────────────
  LAST ERROR ACCESSOR
──────────────────────────────────────────────*/
const ShrijiErrorInfo *shriji_last_error(void)
{
    if (!error_reported)
        return NULL;
    return &LAST_ERROR;
}
