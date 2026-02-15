#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "env.h"
#include "ast.h"
#include "value.h"

//──────────────────────────────────────────────────────────────
// SHRIJI TRUTH DIARY (SINGLE SOURCE OF TRUTH)
//──────────────────────────────────────────────────────────────
typedef struct {
    char last_var[64];
    long long last_value;
    int has_assignment;
    int had_runtime_error;   // ⭐ ONLY error flag
} ShrijiTruthDiary;

// defined in src/interpreter.c
extern ShrijiTruthDiary SHRIJI_DIARY;

//──────────────────────────────────────────────────────────────
// MAIN EVALUATOR
//──────────────────────────────────────────────────────────────
Value eval(ASTNode *node, Env *env);

//──────────────────────────────────────────────────────────────
// PROGRAM ENTRY
//──────────────────────────────────────────────────────────────
Value run_program(ASTNode *program, Env *env);

#endif /* INTERPRETER_H */
