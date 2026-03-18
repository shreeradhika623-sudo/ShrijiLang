#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "env.h"
#include "ast.h"
#include "value.h"
#include "runtime.h"   // 🔥 NEW — runtime struct access

//──────────────────────────────────────────────────────────────
// SHRIJI TRUTH DIARY (SINGLE SOURCE OF TRUTH)
//──────────────────────────────────────────────────────────────
typedef struct {
    char last_var[64];
    long long last_value;
    int has_assignment;
    int had_runtime_error;
} ShrijiTruthDiary;

// defined in src/interpreter.c
extern ShrijiTruthDiary SHRIJI_DIARY;


//──────────────────────────────────────────────────────────────
// MAIN EVALUATOR
//──────────────────────────────────────────────────────────────
//
// 🔥 IMPORTANT CHANGE:
// eval now receives runtime pointer
//
Value eval(ASTNode *node, Env *env, ShrijiRuntime *rt);


//──────────────────────────────────────────────────────────────
// PROGRAM ENTRY
//──────────────────────────────────────────────────────────────
//
// 🔥 IMPORTANT CHANGE:
// run_program now receives runtime pointer
//
Value run_program(ASTNode *program, Env *env, ShrijiRuntime *rt);


#endif /* INTERPRETER_H */
