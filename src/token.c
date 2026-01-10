#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "../include/token.h"

/*──────────────────────────────────────────────
  SHRIJILANG LEXER — FINAL STABLE (NEWLINE AWARE)
  Supports:
    - Newline tokens
    - Keywords: mavi, agar, warna, jabtak, nahi
    - Operators: + - * / % = == != > < >= <=
    - Comments: #... and //...
──────────────────────────────────────────────*/

static const char *start;
static const char *current;

/*──────── helpers ────────*/
static int is_at_end(void) {
    return *current == '\0';
}

static char advance_char(void) {
    return *current++;
}

static char peek(void) {
    return *current;
}

static char peek_next(void) {
    if (is_at_end()) return '\0';
    return current[1];
}

static Token make_token(TokenType type, const char *s, int len) {
    Token t;
    t.type = type;
    t.start = s;
    t.length = len;
    return t;
}

/*──────── skip whitespace (NOT newline) + comments ────────*/
static void skip_ws(void) {
    for (;;) {
        char c = peek();

        /* whitespace (except newline) */
        if (c == ' ' || c == '\r' || c == '\t') {
            advance_char();
            continue;
        }

        /* hash comments: # ... (until newline) */
        if (c == '#') {
            while (peek() != '\n' && !is_at_end())
                advance_char();
            continue;
        }

        /* C++ style comments: // ... (until newline) */
        if (c == '/' && peek_next() == '/') {
            while (peek() != '\n' && !is_at_end())
                advance_char();
            continue;
        }

        return;
    }
}

/*──────── keywords ────────*/
static TokenType check_keyword(const char *s, int len) {
    if (len == 4 && strncmp(s, "mavi", 4) == 0) return TOKEN_MAVI;
    if (len == 4 && strncmp(s, "agar", 4) == 0) return TOKEN_AGAR;
    if (len == 5 && strncmp(s, "warna", 5) == 0) return TOKEN_WARNA;
    if (len == 6 && strncmp(s, "jabtak", 6) == 0) return TOKEN_JABTAK;
    if (len == 4 && strncmp(s, "nahi", 4) == 0) return TOKEN_NAHI;
    if (len == 4 && strncmp(s, "true", 4) == 0) return TOKEN_TRUE;
    if (len == 5 && strncmp(s, "false", 5) == 0) return TOKEN_FALSE;
    return TOKEN_IDENTIFIER;
}

/*──────── identifiers ────────*/
static Token identifier(void) {
    while (isalnum(peek()) || peek() == '_') advance_char();
    int len = (int)(current - start);
    return make_token(check_keyword(start, len), start, len);
}

/*──────── numbers ────────*/
static Token number(void) {
    while (isdigit(peek())) advance_char();
    int len = (int)(current - start);
    return make_token(TOKEN_NUMBER, start, len);
}

/*──────── string ────────*/
static Token string(void) {
    while (peek() != '"' && !is_at_end()) advance_char();
    if (!is_at_end()) advance_char();   /* closing " */
    int len = (int)(current - start);
    return make_token(TOKEN_STRING, start, len);
}

/*──────── public API ────────*/
void init_tokenizer(const char *src) {
    start = src;
    current = src;
}

Token scan_token(void) {
    skip_ws();
    start = current;

    if (is_at_end()) return make_token(TOKEN_EOF, start, 0);

    /* NEWLINE token */
    if (peek() == '\n') {
        advance_char();
        return make_token(TOKEN_NEWLINE, start, 1);
    }

    char c = advance_char();

    if (isalpha(c) || c == '_') return identifier();
    if (isdigit(c)) return number();

    switch (c) {
        case '(': return make_token(TOKEN_LEFT_PAREN, start, 1);
        case ')': return make_token(TOKEN_RIGHT_PAREN, start, 1);
        case '{': return make_token(TOKEN_LEFT_BRACE, start, 1);
        case '}': return make_token(TOKEN_RIGHT_BRACE, start, 1);

        case '+': return make_token(TOKEN_PLUS, start, 1);
        case '-': return make_token(TOKEN_MINUS, start, 1);
        case '*': return make_token(TOKEN_STAR, start, 1);
        case '/': return make_token(TOKEN_SLASH, start, 1);
        case '%': return make_token(TOKEN_MOD, start, 1);

        /* IMPORTANT FIX:
           after consuming first '=', the second '=' is at peek(), NOT peek_next() */
        case '=':
            if (peek() == '=') {
                advance_char();
                return make_token(TOKEN_EQEQ, start, 2);
            }
            return make_token(TOKEN_EQUAL, start, 1);

        /* IMPORTANT FIX: != */
        case '!':
            if (peek() == '=') {
                advance_char();
                return make_token(TOKEN_NEQ, start, 2);
            }
            break;

        /* IMPORTANT FIX: >= */
        case '>':
            if (peek() == '=') {
                advance_char();
                return make_token(TOKEN_GTE, start, 2);
            }
            return make_token(TOKEN_GT, start, 1);

        /* IMPORTANT FIX: <= */
        case '<':
            if (peek() == '=') {
                advance_char();
                return make_token(TOKEN_LTE, start, 2);
            }
            return make_token(TOKEN_LT, start, 1);

        case '"':
            return string();
    }

    return make_token(TOKEN_ERROR, start, 1);
}
