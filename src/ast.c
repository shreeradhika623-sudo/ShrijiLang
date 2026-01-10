#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/ast.h"

/*──────────────────────────────────────────────────────────────
 |  SHRIJILANG — AST CONSTRUCTOR IMPLEMENTATION (CLEAN & STABLE)
 *──────────────────────────────────────────────────────────────*/

//──────────────────────────────────────────────────────────────
// SECTION A — SAFE STRING HELPER
//──────────────────────────────────────────────────────────────
static void safe_copy(char *dest, const char *src, int max) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, max - 1);
    dest[max - 1] = '\0';
}

//──────────────────────────────────────────────────────────────
// SECTION B — CORE NODES
//──────────────────────────────────────────────────────────────
ASTNode *new_number_node(int value) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;
    node->type = AST_NUMBER;
    node->number_value = value;
    node->chakra_state = CHAKRA_OK;
    return node;
}

ASTNode *new_identifier_node(const char *name) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;
    node->type = AST_IDENTIFIER;
    safe_copy(node->name, name, 128);
    node->chakra_state = CHAKRA_OK;
    return node;
}

ASTNode *new_string_node(const char *str) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;
    node->type = AST_STRING;
    safe_copy(node->string_value, str, 256);
    node->chakra_state = CHAKRA_OK;
    return node;
}

ASTNode *new_binary_node(char op, ASTNode *l, ASTNode *r) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;
    node->type = AST_BINARY;
    node->op[0] = op;
    node->left = l;
    node->right = r;
    node->chakra_state = CHAKRA_OK;
    return node;
}

ASTNode *new_assignment_node(const char *name, ASTNode *val) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;
    node->type = AST_ASSIGNMENT;
    safe_copy(node->name, name, 128);
    node->value = val;
    node->chakra_state = CHAKRA_OK;
    return node;
}

//──────────────────────────────────────────────────────────────
// SECTION C — AI NODES
//──────────────────────────────────────────────────────────────
#define AI_NODE(fn, type_id)                  \
ASTNode *fn(ASTNode *msg) {                  \
    ASTNode *node = calloc(1, sizeof(ASTNode)); \
    if (!node) return NULL;                  \
    node->type = type_id;                    \
    node->value = msg;                       \
    node->chakra_state = CHAKRA_OK;          \
    return node;                             \
}

AI_NODE(new_sakhi_node, AST_SAKHI)
AI_NODE(new_niyu_node,  AST_NIYU)
AI_NODE(new_shiri_node, AST_SHIRI)
AI_NODE(new_mira_node,  AST_MIRA)
AI_NODE(new_kavya_node, AST_KAVYA)

//──────────────────────────────────────────────────────────────
// SECTION D — COMMAND NODE
//──────────────────────────────────────────────────────────────
ASTNode *new_command_node(const char *cmd, ASTNode *arg) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;
    node->type = AST_COMMAND;
    safe_copy(node->command_name, cmd, 128);
    node->value = arg;
    node->chakra_state = CHAKRA_OK;
    return node;
}

//──────────────────────────────────────────────────────────────
// SECTION D - A  — IMPORT NODE (MODULE LOAD DECLARATION)
//──────────────────────────────────────────────────────────────
ASTNode *new_import_node(const char *module_name) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;

    node->type = AST_IMPORT;
    safe_copy(node->name, module_name, 128);

    node->chakra_state = CHAKRA_OK;
    return node;
}

//──────────────────────────────────────────────────────────────
// SECTION D.1 — BLOCK NODE (ONLY ONE, CLEAN)
//──────────────────────────────────────────────────────────────
ASTNode *new_block_node(ASTNode **stmts, int count) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;
    node->type = AST_BLOCK;
    node->statements = stmts;
    node->stmt_count = count;
    node->chakra_state = CHAKRA_OK;
    return node;
}

//──────────────────────────────────────────────────────────────
// SECTION D.1.1 — PROGRAM ROOT NODE (SCRIPT ENTRY)
//──────────────────────────────────────────────────────────────
ASTNode *new_program_node(ASTNode **stmts, int count) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;

    node->type = AST_PROGRAM;
    node->statements = stmts;
    node->stmt_count = count;

    node->chakra_state = CHAKRA_OK;
    return node;
}

//──────────────────────────────────────────────────────────────
// SECTION D.2 — IF NODE (GRAMMAR SKELETON ONLY)
//──────────────────────────────────────────────────────────────
ASTNode *new_if_node(ASTNode *condition, ASTNode *then_block) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;

    node->type = AST_IF;
    node->condition = condition;
    node->left = then_block;   // then-block
    node->chakra_state = CHAKRA_OK;

    return node;
}

//──────────────────────────────────────────────────────────────
// SECTION D.2.1 — WHILE LOOP NODE (jabtak)
//──────────────────────────────────────────────────────────────
ASTNode *new_while_node(ASTNode *condition, ASTNode *body) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;

    node->type = AST_WHILE;
    node->condition = condition;
    node->body = body;

    node->chakra_state = CHAKRA_OK;
    return node;
}

//──────────────────────────────────────────────────────────────
// SECTION D.3 — FUNCTION NODE
//──────────────────────────────────────────────────────────────
ASTNode *new_function_node(
    const char *name,
    ASTNode **params,
    int param_count,
    ASTNode *body
) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;

    node->type = AST_FUNCTION;
    safe_copy(node->function_name, name, 128);

    node->params = params;
    node->param_count = param_count;
    node->body = body;

    node->chakra_state = CHAKRA_OK;
    return node;
}

//──────────────────────────────────────────────────────────────
// SECTION D.4 — FUNCTION CALL NODE
//──────────────────────────────────────────────────────────────
ASTNode *new_call_node(
    const char *name,
    ASTNode **args,
    int arg_count
) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;

    node->type = AST_CALL;
    safe_copy(node->function_name, name, 128);

    node->args = args;
    node->arg_count = arg_count;

    node->chakra_state = CHAKRA_OK;
    return node;
}

//──────────────────────────────────────────────────────────────
// SECTION D.5 — RETURN NODE
//──────────────────────────────────────────────────────────────
ASTNode *new_return_node(ASTNode *expr) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;

    node->type = AST_RETURN;
    node->return_expr = expr;

    node->chakra_state = CHAKRA_OK;
    return node;
}

//──────────────────────────────────────────────
// SECTION E — MEMORY CLEANUP (FUNCTION AWARE)
//──────────────────────────────────────────────
void ast_free(ASTNode *node) {
    if (!node) return;

    /* Generic child nodes */
    ast_free(node->left);
    ast_free(node->right);
    ast_free(node->value);
    ast_free(node->condition);
    ast_free(node->else_block);

    /* Function body */
    ast_free(node->body);

    /* Return expression */
    ast_free(node->return_expr);

    /* Block statements */
    if (node->statements) {
        for (int i = 0; i < node->stmt_count; i++) {
            ast_free(node->statements[i]);
        }
        free(node->statements);
    }

    /* Function parameters */
    if (node->params) {
        for (int i = 0; i < node->param_count; i++) {
            ast_free(node->params[i]);
        }
        free(node->params);
    }

    /* Call arguments */
    if (node->args) {
        for (int i = 0; i < node->arg_count; i++) {
            ast_free(node->args[i]);
        }
        free(node->args);
    }

    free(node);
}

//──────────────────────────────────────────────
// LOGICAL NOT NODE (nahi)
//──────────────────────────────────────────────
ASTNode *new_not_node(ASTNode *expr) {

    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) return NULL;

    node->type = AST_NOT;
    node->left = expr;      // unary operand
    node->right = NULL;

    node->chakra_state = CHAKRA_OK;

    return node;
}
