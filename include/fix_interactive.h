#ifndef SHRIJI_FIX_INTERACTIVE_H
#define SHRIJI_FIX_INTERACTIVE_H

#include <stddef.h>
#include "error.h"


/* =========================================
   INTERACTIVE OPERATOR FIX
   Example: 5 5 → 5 + 5
   ========================================= */

int shriji_interactive_operator_fix(
    const char *input,
    char *output,
    int size
);


/* =========================================
   INTERACTIVE BRACKET FIX
   Example: (5+6 → (5+6)
            (5+6 → 5+6
   ========================================= */

int shriji_interactive_bracket_fix(
    const char *input,
    char *output,
    int size
);

int shriji_interactive_double_operator_fix(
    const char *input,
    char *output,
    int size
);

int fix_missing_value_after_equal_interactive(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err
);
#endif
