#include <stdio.h>
#include "../include/error.h"

//──────────────────────────────────────────────
// INTERNAL: error code → string
//──────────────────────────────────────────────
static const char *error_code_str(ShrijiErrorCode code) {
    switch (code) {
        case E_ASSIGN_01: return "E_ASSIGN_01";
        case E_PARSE_01:  return "E_PARSE_01";
        case E_PARSE_02:  return "E_PARSE_02";
        case E_IF_01:     return "E_IF_01";
        default:          return "E_UNKNOWN";
    }
}

//──────────────────────────────────────────────
// SHRIJI ERROR EMITTER (POLISHED V1)
//──────────────────────────────────────────────
void shriji_error(
    ShrijiErrorCode code,
    const char *context,
    const char *message,
    const char *hint
) {
    printf("\n⛔ %s | %s → %s\n",
           error_code_str(code),
           context,
           message);

    if (hint && hint[0] != '\0') {
        printf("   hint: %s\n", hint);
    }

    printf("\n");
}
