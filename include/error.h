#ifndef ERROR_H
#define ERROR_H

#include "../include/token.h"

/*──────────────────────────────────────────────
  SHRIJI ERROR CODES (L3 STABLE)
──────────────────────────────────────────────*/
typedef enum {

    /* Assignment / Declaration */
    E_ASSIGN_01,

    /* Parse Errors */
    E_PARSE_01,
    E_PARSE_02,
    E_IF_01,

    /* Runtime Errors */
    E_RUNTIME_01

} ShrijiErrorCode;

/*──────────────────────────────────────────────
  L3 — ERROR SNAPSHOT (EXPLAINABILITY READY)
──────────────────────────────────────────────*/
typedef struct {
    ShrijiErrorCode code;
    const char *context;
    const char *message;
    const char *hint;

    /* token location (optional) */
    int has_location;
    int line;
    int col;
} ShrijiErrorInfo;

/*──────────────────────────────────────────────
  CENTRAL ERROR EMITTERS
──────────────────────────────────────────────*/
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

/*──────────────────────────────────────────────
  LAST ERROR ACCESS (READ ONLY)
──────────────────────────────────────────────*/
const ShrijiErrorInfo *shriji_last_error(void);

/*──────────────────────────────────────────────
  GLOBAL ERROR FLAG
──────────────────────────────────────────────*/
extern int error_reported;

#endif /* ERROR_H */
