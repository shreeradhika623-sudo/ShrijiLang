#ifndef ERROR_H
#define ERROR_H

//──────────────────────────────────────────────
// SHRIJI ERROR CODES (LOCKED V1)
//──────────────────────────────────────────────
typedef enum {
    E_ASSIGN_01,
    E_PARSE_01,
    E_PARSE_02,
    E_IF_01
} ShrijiErrorCode;

//──────────────────────────────────────────────
// CENTRAL ERROR EMITTER
//──────────────────────────────────────────────
void shriji_error(
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
);

#endif
