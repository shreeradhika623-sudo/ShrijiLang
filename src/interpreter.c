#include <stdio.h>
#include <string.h>

#include "../include/interpreter.h"
#include "../include/ast.h"
#include "../include/env.h"
#include "../include/error.h"
#include "../include/state.h"
#include "../include/event.h"
#include "../include/command.h"

#include "../shiri/shiri.h"

/*──────────────────────────────────────────────────────────────
 | SHRIJI TRUTH DIARY — SINGLE SOURCE OF FACT
 *──────────────────────────────────────────────────────────────*/
ShrijiTruthDiary SHRIJI_DIARY = {0};

/*──────────────────────────────────────────────────────────────
 | GLOBAL EXECUTION STATE
 *──────────────────────────────────────────────────────────────*/
ExecutionState SHRIJI_STATE;

/*──────────────────────────────────────────────────────────────
 | STATE DECISION HOOK
 *──────────────────────────────────────────────────────────────*/
static int state_allows_execution(void)
{
    if (SHRIJI_STATE.safety == STATE_CRITICAL) {

        const char *out = shiri_process("system:halted");
        if (out && *out)
            printf("%s\n", out);

        return 0;
    }
    return 1;
}

/*──────────────────────────────────────────────────────────────
 | SAFE DIVISION
 *──────────────────────────────────────────────────────────────*/
static long long safe_div(long long a, long long b)
{
    if (b == 0) {

        event_fire(EVENT_ERROR, "division by zero");

        shriji_error(
            E_PARSE_02,
            "division",
            "division by zero",
            "denominator must not be zero"
        );
        return 0;
    }
    return a / b;
}

/*──────────────────────────────────────────────────────────────
 | SAFE MODULO
 *──────────────────────────────────────────────────────────────*/
static long long safe_mod(long long a, long long b)
{
    if (b == 0) {

        event_fire(EVENT_ERROR, "modulo by zero");

        shriji_error(
            E_PARSE_02,
            "modulo",
            "modulo by zero",
            "denominator must not be zero"
        );
        return 0;
    }
    return a % b;
}

/*──────────────────────────────────────────────────────────────
 | MAIN EVALUATOR
 *──────────────────────────────────────────────────────────────*/
long long eval(ASTNode *node, Env *env)
{
    static int state_initialized = 0;
    if (!state_initialized) {
        state_init(&SHRIJI_STATE);
        state_initialized = 1;
    }

    /* 🔐 Global safety gate */
    if (!state_allows_execution())
        return 0;

    if (!node)
        return 0;

    /* 🧠 STEP COUNTER (runtime guard) */
    SHRIJI_STATE.steps_used++;

    if (SHRIJI_STATE.steps_used > SHRIJI_STATE.max_steps) {
        shriji_error(
            E_PARSE_02,
            "runtime",
            "execution limit exceeded",
            "possible infinite loop detected"
        );
        SHRIJI_STATE.safety = STATE_CRITICAL;
        return 0;
    }

    switch (node->type) {

    /*──────── NUMBER ────────*/
    case AST_NUMBER:
        return node->number_value;

    /*──────── IDENTIFIER ────────*/
    case AST_IDENTIFIER: {

        if (!env_exists(env, node->name)) {

            char event[64];
            snprintf(event, sizeof(event),
                     "shunya:%.*s", 48, node->name);

            const char *out = shiri_process(event);
            if (out && *out)
                printf("%s\n", out);

            return 0;
        }
        return env_get(env, node->name);
    }

    /*──────── ASSIGNMENT ────────*/
    case AST_ASSIGNMENT: {

        long long value = eval(node->value, env);
        env_set(env, node->name, value);

        event_fire(EVENT_ASSIGNMENT, node->name);
        state_on_success(&SHRIJI_STATE);

        strncpy(
            SHRIJI_DIARY.last_var,
            node->name,
            sizeof(SHRIJI_DIARY.last_var) - 1
        );
        SHRIJI_DIARY.last_var[sizeof(SHRIJI_DIARY.last_var) - 1] = '\0';

        SHRIJI_DIARY.last_value = value;
        SHRIJI_DIARY.has_assignment = 1;

        char msg[64];
        snprintf(msg, sizeof(msg), "result:%lld", value);

        const char *out = shiri_process(msg);
        if (out && *out)
            printf("%s\n", out);

        return value;
    }

    /*──────── UPDATE (x = 10) ────────*/
    case AST_UPDATE: {

        if (!env_exists(env, node->name)) {
            shriji_error(
                E_ASSIGN_01,
                node->name,
                "variable not declared",
                "use mavi x = ... first"
            );
            return 0;
        }

        long long value = eval(node->value, env);
        env_set(env, node->name, value);

        event_fire(EVENT_ASSIGNMENT, node->name);
        state_on_success(&SHRIJI_STATE);

        char msg[64];
        snprintf(msg, sizeof(msg), "update:%lld", value);

        const char *out = shiri_process(msg);
        if (out && *out)
            printf("%s\n", out);

        return value;
    }

    /*──────── BINARY ────────*/
    case AST_BINARY: {

        long long L = eval(node->left, env);
        long long R = eval(node->right, env);

        /* guard */
        char op = 0;
        if (node->op && node->op[0])
            op = node->op[0];

        switch (op) {
            case '+': return L + R;
            case '-': return L - R;
            case '*': return L * R;
            case '/': return safe_div(L, R);
            case '%': return safe_mod(L, R);

            case '>': return (L > R);
            case '<': return (L < R);
            case '=': return (L == R);
            case '!': return (L != R);

            default:
                shriji_error(
                    E_PARSE_02,
                    "binary",
                    "unknown operator",
                    "check expression"
                );
                return 0;
        }
    }

    /*──────── LOGICAL NOT ────────*/
    case AST_NOT: {
        long long v = eval(node->left, env);
        return !v;
    }

    /*──────── PROGRAM ────────*/
    case AST_PROGRAM: {
        long long last = 0;
        for (int i = 0; i < node->stmt_count; i++) {
            last = eval(node->statements[i], env);
        }
        return last;
    }

    /*──────── BLOCK ────────*/
    case AST_BLOCK: {
        long long last = 0;
        for (int i = 0; i < node->stmt_count; i++) {
            last = eval(node->statements[i], env);
        }
        return last;
    }

    /*──────── IF / WARNA ────────*/
    case AST_IF: {

        long long cond = eval(node->condition, env);

        if (cond) {
            return eval(node->left, env);
        } else if (node->else_block) {
            return eval(node->else_block, env);
        }
        return 0;
    }

    /*──────── COMMAND ────────*/
    case AST_COMMAND: {

        CommandType cmd = resolve_command(node->command_name);

        if (cmd == CMD_UNKNOWN) {
            shriji_error(
                E_PARSE_02,
                "command",
                "unknown command",
                "command not recognized"
            );
            return 0;
        }

        int status = execute_command(cmd);

        char msg[128];
        snprintf(
            msg,
            sizeof(msg),
            "Command executed internally as %s",
            command_name(cmd)
        );

        const char *out = shiri_process(msg);
        if (out && *out)
            printf("%s\n", out);

        return status;
    }

    default:
        return 0;
    }
}

/*──────────────────────────────────────────────────────────────
 | PROGRAM RUNNER
 *──────────────────────────────────────────────────────────────*/
long long run_program(ASTNode *program, Env *env)
{
    if (!program || program->type != AST_PROGRAM)
        return 0;

    long long last = 0;
    for (int i = 0; i < program->stmt_count; i++) {
        last = eval(program->statements[i], env);
    }
    return last;
}
