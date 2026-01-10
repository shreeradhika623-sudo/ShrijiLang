#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parser.h"
#include "../include/token.h"
#include "../include/ast.h"
#include "../include/error.h"

/*──────────────────────────────────────────────
 | SHRIJILANG PARSER — SCRIPT + REPL SAFE
 | Supports: mavi, x =, agar, warna, jabtak, blocks
 *──────────────────────────────────────────────*/

static ASTNode *parse_statement(void);
static ASTNode *parse_block(void);
static ASTNode *parse_if(void);
static ASTNode *parse_while(void);
static ASTNode *parse_assignment(void);
static ASTNode *parse_update_or_identifier(void);

static ASTNode *parse_expression(void);
static ASTNode *parse_comparison(void);
static ASTNode *parse_term(void);
static ASTNode *parse_unary(void);
static ASTNode *parse_primary(void);

static Token current;

/*──────── helpers ────────*/
static void advance(void) { current = scan_token(); }

static int expect(TokenType t, ShrijiErrorCode c,
                  const char *ctx, const char *msg, const char *hint)
{
    if (current.type == t) {
        advance();
        return 1;
    }
    shriji_error(c, ctx, msg, hint);
    return 0;
}

/* allow safe statement boundaries (future semicolons etc.) */
static void skip_separators(void)
{
    while (current.type == TOKEN_NEWLINE) {
        advance();
    }
}

/*──────── program ────────*/
ASTNode *parse_program(const char *source)
{
    init_tokenizer(source);
    advance();

    ASTNode **stmts = NULL;
    int count = 0;

    while (current.type != TOKEN_EOF) {

        skip_separators();
        if (current.type == TOKEN_EOF) break;

        ASTNode *s = parse_statement();
        if (!s) return NULL;

        stmts = realloc(stmts, sizeof(ASTNode*) * (count + 1));
        stmts[count++] = s;

        skip_separators();
    }

    return new_program_node(stmts, count);
}

/*──────── statements ────────*/
static ASTNode *parse_statement(void)
{
    skip_separators();

    if (current.type == TOKEN_LEFT_BRACE) return parse_block();
    if (current.type == TOKEN_AGAR)       return parse_if();
    if (current.type == TOKEN_JABTAK)     return parse_while();
    if (current.type == TOKEN_MAVI)       return parse_assignment();

    /* Identifier: UPDATE (x = expr) OR just identifier expression */
    if (current.type == TOKEN_IDENTIFIER) {
        return parse_update_or_identifier();
    }

    /* number, true, false, (expr) also allowed */
    if (current.type == TOKEN_NUMBER ||
        current.type == TOKEN_TRUE ||
        current.type == TOKEN_FALSE ||
        current.type == TOKEN_LEFT_PAREN) {
        return parse_expression();
    }

    shriji_error(
        E_PARSE_02,
        "statement",
        "invalid statement",
        "use mavi / agar / jabtak / { } or expression"
    );
    return NULL;
}

/*──────── block ────────*/
static ASTNode *parse_block(void)
{
    /* must start with { */
    if (!expect(TOKEN_LEFT_BRACE, E_PARSE_01, "{", "missing {", "{ stmt }"))
        return NULL;

    ASTNode **stmts = NULL;
    int count = 0;

    while (current.type != TOKEN_RIGHT_BRACE &&
           current.type != TOKEN_EOF) {

        skip_separators();

        if (current.type == TOKEN_RIGHT_BRACE)
            break;

        ASTNode *s = parse_statement();
        if (!s) return NULL;

        stmts = realloc(stmts, sizeof(ASTNode*) * (count + 1));
        stmts[count++] = s;

        skip_separators();
    }

    /* must end with } */
    if (!expect(TOKEN_RIGHT_BRACE, E_PARSE_02, "}", "missing }", "{ stmt }"))
        return NULL;

    return new_block_node(stmts, count);
}

/*──────── if / agar ────────*/
static ASTNode *parse_if(void)
{
    advance(); /* consume 'agar' */

    ASTNode *cond = parse_expression();
    if (!cond) return NULL;

    ASTNode *then_block = parse_block();
    if (!then_block) return NULL;

    ASTNode *node = new_if_node(cond, then_block);

    if (current.type == TOKEN_WARNA) {
        advance();
        node->else_block = parse_block();
        if (!node->else_block) return NULL;
    }

    return node;
}

/*──────── while / jabtak ────────*/
static ASTNode *parse_while(void)
{
    advance(); /* consume 'jabtak' */

    ASTNode *cond = parse_expression();
    if (!cond) return NULL;

    ASTNode *body = parse_block();
    if (!body) return NULL;

    return new_while_node(cond, body);
}

/*── parse_assignment ── mavi x = expr ────────*/
static ASTNode *parse_assignment(void)
{
    advance(); /* consume 'mavi' */
    skip_separators();

    if (current.type != TOKEN_IDENTIFIER) {
        shriji_error(E_ASSIGN_01, "mavi", "expected variable name", "mavi x = 10");
        return NULL;
    }

    char name[128];
    strncpy(name, current.start, current.length);
    name[current.length] = 0;
    advance(); /* consume identifier */

    skip_separators();

    if (!expect(TOKEN_EQUAL, E_ASSIGN_01, "=", "missing =", "mavi x = 10"))
        return NULL;

    ASTNode *val = parse_expression();
    if (!val) return NULL;

    skip_separators();
    return new_assignment_node(name, val);
}

/*── parse_update_or_identifier ── x = expr OR just x ────────
   IMPORTANT: no lookahead/rewind. Lexer state must remain correct. */
static ASTNode *parse_update_or_identifier(void)
{
    if (current.type != TOKEN_IDENTIFIER)
        return NULL;

    char name[128];
    strncpy(name, current.start, current.length);
    name[current.length] = 0;
    advance(); /* consume identifier */

    skip_separators();

    /* If next token isn't '=', then this statement is just identifier expression */
    if (current.type != TOKEN_EQUAL) {
        return new_identifier_node(name);
    }

    advance(); /* consume '=' */

    ASTNode *val = parse_expression();
    if (!val) return NULL;

    skip_separators();
    return new_assignment_node(name, val);
}

/*──────── expressions ────────*/
static ASTNode *parse_expression(void)
{
    ASTNode *n = parse_comparison();
    while (current.type == TOKEN_PLUS || current.type == TOKEN_MINUS) {
        char op = (current.type == TOKEN_PLUS) ? '+' : '-';
        advance();
        n = new_binary_node(op, n, parse_comparison());
    }
    return n;
}

static ASTNode *parse_comparison(void)
{
    ASTNode *n = parse_term();
    while (current.type == TOKEN_GT  || current.type == TOKEN_LT ||
           current.type == TOKEN_EQEQ || current.type == TOKEN_NEQ) {

        char op =
            (current.type == TOKEN_GT)   ? '>' :
            (current.type == TOKEN_LT)   ? '<' :
            (current.type == TOKEN_EQEQ) ? '=' : '!';

        advance();
        n = new_binary_node(op, n, parse_term());
    }
    return n;
}

static ASTNode *parse_term(void)
{
    ASTNode *n = parse_unary();
    while (current.type == TOKEN_STAR ||
           current.type == TOKEN_SLASH ||
           current.type == TOKEN_MOD) {

        char op =
            (current.type == TOKEN_STAR)  ? '*' :
            (current.type == TOKEN_SLASH) ? '/' : '%';

        advance();
        n = new_binary_node(op, n, parse_unary());
    }
    return n;
}

static ASTNode *parse_unary(void)
{
    if (current.type == TOKEN_NAHI) {
        advance();
        return new_not_node(parse_unary());
    }
    return parse_primary();
}

/*──────── primary ────────*/
static ASTNode *parse_primary(void)
{
    if (current.type == TOKEN_LEFT_BRACE)
        return NULL;

    if (current.type == TOKEN_NUMBER) {
        int v = atoi(current.start);
        advance();
        return new_number_node(v);
    }

    if (current.type == TOKEN_TRUE) {
        advance();
        return new_number_node(1);
    }

    if (current.type == TOKEN_FALSE) {
        advance();
        return new_number_node(0);
    }

    if (current.type == TOKEN_IDENTIFIER) {
        char name[128];
        strncpy(name, current.start, current.length);
        name[current.length] = 0;
        advance();
        return new_identifier_node(name);
    }

    if (current.type == TOKEN_LEFT_PAREN) {
        advance();
        ASTNode *n = parse_expression();
        if (!n) return NULL;
        expect(TOKEN_RIGHT_PAREN, E_PARSE_02, ")", "missing )", "(expr)");
        return n;
    }

    shriji_error(E_PARSE_02, "expr", "bad token", "check syntax");
    return NULL;
}
