#ifndef SHRIJI_FIX_ENGINE_H
#define SHRIJI_FIX_ENGINE_H

#include "ast.h"

/*
    Language execution with controlled fix
    ---------------------------------------
    input        → original user input
    was_fixed    → 1 if fix applied
    penalty_out  → confidence penalty from rule
*/

ASTNode *language_execute_with_fix(
    const char *input,
    int *was_fixed,
    int *penalty_out
);

#endif
