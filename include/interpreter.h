#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "env.h"
#include "ast.h"

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
long long eval(ASTNode *node, Env *env);

//──────────────────────────────────────────────────────────────
// PROGRAM ENTRY (STEP-7.1)
//──────────────────────────────────────────────────────────────
long long run_program(ASTNode *program, Env *env);
#endif
