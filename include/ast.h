#ifndef AST_H
#define AST_H

/*──────────────────────────────────────────────────────────────
 |  SHRIJILANG — ABSTRACT SYNTAX TREE (AST)
 |
 |  Used by:
 |   • Parser
 |   • Interpreter
 |   • AI Engines (Sakhi / Niyu / Mira / Kavya / Shiri)
 |   • Universal Command Engine (sri_*)
 |   • Future Grammar (block, if, loop, function)
 *────────────────────────────────────────────────────────────*/

/*──────────────────────────────────────────────────────────────
 |  SECTION A — CHAKRA DIAGNOSTIC ENGINE
 *────────────────────────────────────────────────────────────*/
typedef enum {
    CHAKRA_OK = 0,
    CHAKRA_MOOLADHARA,
    CHAKRA_SWADHISTHAN,
    CHAKRA_MANIPUR,
    CHAKRA_ANAHATA,
    CHAKRA_VISHUDDHI,
    CHAKRA_AJNA,
    CHAKRA_SAHASRARA
} ChakraState;


/*──────────────────────────────────────────────────────────────
 |  SECTION B — AST NODE TYPES
 *────────────────────────────────────────────────────────────*/
typedef enum {

    /* Core literals */
    AST_NUMBER,
    AST_IDENTIFIER,
    AST_STRING,
    AST_BOOL,        // true / false
    /* Expressions */
    AST_BINARY,
    AST_ASSIGNMENT,
    AST_UPDATE,        // x = expression
    /* Control / Grammar */
    AST_IF,
    AST_BLOCK,
    AST_PROGRAM,     // script / file root
    AST_IMPORT,      // module import
    AST_LOOP,        // future
    AST_WHILE,       // jabtak loop
    AST_NOT,    //logical NOT (Nahi)
    /* AI Nodes */
    AST_SAKHI,
    AST_NIYU,
    AST_SHIRI,
    AST_MIRA,
    AST_KAVYA,

/* Functions */
    AST_FUNCTION,      // function definition
    AST_CALL,          // function call
    AST_RETURN,        // return statement

    /* Universal command (sri_*) */
    AST_COMMAND

} ASTNodeType;


/*─────────────────────────────────────────────────────────────
 | SECTION C — AST NODE STRUCTURE
 *─────────────────────────────────────────────────────────────*/
typedef struct ASTNode {

    ASTNodeType type;

    /*────────────────────────
      Primitive values
    ────────────────────────*/
    int  number_value;
    int  bool_value;     // 1 = true, 0 = false
    char string_value[256];
    char name[128];

    /*────────────────────────
      Binary expression
    ────────────────────────*/
    struct ASTNode *left;
    struct ASTNode *right;
    char op[4];                 // + - * / > < >= <= == !=

    /*────────────────────────
      Assignment / Command
    ────────────────────────*/
    struct ASTNode *value;
    char command_name[128];

    /*────────────────────────
      IF / ELSE
    ────────────────────────*/
    struct ASTNode *condition;   // agar condition
    struct ASTNode *else_block;  // warna block

    /*────────────────────────
      Block
    ────────────────────────*/
    struct ASTNode **statements;
    int stmt_count;

/*────────────────────────
      Function
    ────────────────────────*/
    char function_name[128];

    struct ASTNode **params;
    int param_count;

    struct ASTNode *body;

    /*────────────────────────
      Call
    ────────────────────────*/
    struct ASTNode **args;
    int arg_count;

    /*────────────────────────
      Return
    ────────────────────────*/
    struct ASTNode *return_expr;

    /*────────────────────────
      Diagnostics
    ────────────────────────*/
    ChakraState chakra_state;

} ASTNode;


/*──────────────────────────────────────────────────────────────
 |  SECTION D — CONSTRUCTORS
 *────────────────────────────────────────────────────────────*/

/* Core */
ASTNode *new_number_node(int value);
ASTNode *new_identifier_node(const char *name);
ASTNode *new_string_node(const char *str);

/* Expressions */
ASTNode *new_binary_node(char op, ASTNode *left, ASTNode *right);
ASTNode *new_assignment_node(const char *name, ASTNode *value);

/* AI */
ASTNode *new_sakhi_node(ASTNode *msg);
ASTNode *new_niyu_node(ASTNode *msg);
ASTNode *new_shiri_node(ASTNode *msg);
ASTNode *new_mira_node(ASTNode *msg);
ASTNode *new_kavya_node(ASTNode *msg);

/* Universal command */
ASTNode *new_command_node(const char *cmd_name, ASTNode *arg);
ASTNode *new_import_node(const char *module_name);

/* Block */
ASTNode *new_block_node(ASTNode **stmts, int count);
ASTNode *new_program_node(ASTNode **stmts, int count);

/* Grammar */
ASTNode *new_if_node(ASTNode *condition, ASTNode *then_block);
ASTNode *new_while_node(ASTNode *condition, ASTNode *body);

/* Functions */
ASTNode *new_function_node(
    const char *name,
    ASTNode **params,
    int param_count,
    ASTNode *body
);

ASTNode *new_call_node(
    const char *name,
    ASTNode **args,
    int arg_count
);

ASTNode *new_return_node(ASTNode *expr);

/* Cleanup */
void ast_free(ASTNode *node);

/* Logical */
ASTNode *new_not_node(ASTNode *expr);

#endif /* AST_H */
