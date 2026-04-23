/*=============================================================
  SHRIJILANG — TOKENIZER (PRODUCTION STABLE VERSION)
=============================================================*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../include/token.h"

/*──────────────────────────────────────────────
  INTERNAL STATE
──────────────────────────────────────────────*/

static const char *source = NULL;
static int current = 0;
static int line = 1;
static int col  = 1;

/*──────────────────────────────────────────────
  HELPERS
──────────────────────────────────────────────*/

static char peek(void)
{
    return source[current];
}

static char peek_next(void)
{
    if (source[current] == '\0') return '\0';
    return source[current + 1];
}

static char advance(void)
{
    char c = source[current++];

    if (c == '\n') {
        line++;
        col = 1;
    } else {
        col++;
    }

    return c;
}

static int match(char expected)
{
    if (peek() != expected) return 0;
    advance();
    return 1;
}

static Token make_token(TokenType type, const char *start, int length)
{
    Token tok;
    tok.type   = type;
    tok.start  = start;
    tok.length = length;
    tok.line   = line;
    tok.col    = col - length;
    return tok;
}

/*──────────────────────────────────────────────
  SKIP WHITESPACE + COMMENTS
──────────────────────────────────────────────*/

static void skip_whitespace(void)
{
    for (;;) {
        char c = peek();

        switch (c) {

            case ' ':
            case '\r':
            case '\t':
                advance();
                break;

            case '\n':
                advance();
                return;

            case '#':
                while (peek() != '\n' && peek() != '\0') {
                    advance();
                }
                if (peek() == '\n') advance();
                break;

            default:
                return;
        }
    }
}

/*──────────────────────────────────────────────
  NUMBER
──────────────────────────────────────────────*/

static Token number(void)
{
    const char *start = &source[current - 1];

    while (isdigit(peek()))
        advance();

    if (peek() == '.' && isdigit(peek_next())) {
        advance();
        while (isdigit(peek()))
            advance();
    }

    int length = &source[current] - start;
    return make_token(TOKEN_NUMBER, start, length);
}

/*──────────────────────────────────────────────
  IDENTIFIER / KEYWORD
──────────────────────────────────────────────*/

static TokenType check_keyword(const char *start, int length)
{
    if (length == 4 && strncmp(start, "mavi", 4) == 0) return TOKEN_MAVI;
    if (length == 4 && strncmp(start, "agar", 4) == 0) return TOKEN_AGAR;
    if (length == 5 && strncmp(start, "warna", 5) == 0) return TOKEN_WARNA;
    if (length == 4 && strncmp(start, "kaam", 4) == 0) return TOKEN_KAAM;
    if (length == 6 && strncmp(start, "jabtak", 6) == 0) return TOKEN_JABTAK;
    if (length == 5 && strncmp(start, "rukja", 5) == 0) return TOKEN_RUKJA;
    if (length == 5 && strncmp(start, "chalu", 5) == 0) return TOKEN_CHALU;
    if (length == 4 && strncmp(start, "nahi", 4) == 0) return TOKEN_NAHI;
    if (length == 4 && strncmp(start, "sahi", 4) == 0) return TOKEN_TRUE;
    if (length == 5 && strncmp(start, "galat", 5) == 0) return TOKEN_FALSE;


    if (length == 4 && strncmp(start, "bolo", 4) == 0) return TOKEN_BOLO;
    if (length == 6 && strncmp(start, "rachna", 6) == 0) return TOKEN_RACHNA;
    if (length == 5 && strncmp(start, "wapas", 5) == 0) return TOKEN_WAPAS;


    if (length == 5 && strncmp(start, "sakhi", 5) == 0) return TOKEN_SAKHI;
    if (length == 4 && strncmp(start, "niyu", 4) == 0) return TOKEN_NIYU;
    if (length == 4 && strncmp(start, "mira", 4) == 0) return TOKEN_MIRA;
    if (length == 5 && strncmp(start, "kavya", 5) == 0) return TOKEN_KAVYA;
    if (length == 5 && strncmp(start, "shiri", 5) == 0) return TOKEN_SHIRI;

    if (length == 6 && strncmp(start, "import", 6) == 0) return TOKEN_IMPORT;

    return TOKEN_IDENTIFIER;
}

static Token identifier(void)
{
    const char *start = &source[current - 1];

    while (isalnum(peek()) || peek() == '_')
        advance();

    int length = &source[current] - start;
    TokenType type = check_keyword(start, length);

    return make_token(type, start, length);
}

/*──────────────────────────────────────────────
  STRING
──────────────────────────────────────────────*/

static Token string(void)
{
    const char *start = &source[current - 1];

    while (peek() != '"' && peek() != '\0') {
        advance();
    }

    if (peek() == '\0') {
        return make_token(TOKEN_ERROR, start, 0);
    }

    advance();

    int length = &source[current] - start;
    return make_token(TOKEN_STRING, start, length);
}

/*──────────────────────────────────────────────
  INIT
──────────────────────────────────────────────*/

void init_tokenizer(const char *src)
{
    source  = src;
    current = 0;
    line    = 1;
    col     = 1;
}

/*──────────────────────────────────────────────
  MAIN SCANNER
──────────────────────────────────────────────*/

Token scan_token(void)
{
    skip_whitespace();

    const char *start = &source[current];

    if (peek() == '\0')
        return make_token(TOKEN_EOF, start, 0);

    char c = advance();

    if (isdigit(c))
        return number();

    if (isalpha(c) || c == '_')
        return identifier();

    switch (c) {

        case '+':
         if (match('+')) return make_token(TOKEN_PLUSPLUS, start, 2);
                   return make_token(TOKEN_PLUS, start, 1);

        case '-':
         if (match('-')) return make_token(TOKEN_MINUSMINUS, start, 2);
                   return make_token(TOKEN_MINUS, start, 1);

        case '*': return make_token(TOKEN_STAR, start, 1);
        case '/': return make_token(TOKEN_SLASH, start, 1);
        case '&':
    if (match('&')) return make_token(TOKEN_AND, start, 2);
             break;

        case '|':
    if (match('|')) return make_token(TOKEN_OR, start, 2);
             break;

        case '%': return make_token(TOKEN_MOD, start, 1);

        case '(': return make_token(TOKEN_LEFT_PAREN, start, 1);
        case ')': return make_token(TOKEN_RIGHT_PAREN, start, 1);
        case '{': return make_token(TOKEN_LEFT_BRACE, start, 1);
        case '}': return make_token(TOKEN_RIGHT_BRACE, start, 1);
        case '[': return make_token(TOKEN_LEFT_BRACKET, start, 1);
        case ']': return make_token(TOKEN_RIGHT_BRACKET, start, 1);

        case ',': return make_token(TOKEN_COMMA, start, 1);
        case ':': return make_token(TOKEN_COLON, start, 1);

        case '=':
            return make_token(match('=') ? TOKEN_EQEQ : TOKEN_EQUAL, start, 1);

        case '!':
            return make_token(match('=') ? TOKEN_NEQ : TOKEN_NOT, start, 1);

        case '>':
            return make_token(match('=') ? TOKEN_GTE : TOKEN_GT, start, 1);

        case '<':
            return make_token(match('=') ? TOKEN_LTE : TOKEN_LT, start, 1);

        case '\n':
            return make_token(TOKEN_NEWLINE, start, 1);

        case '"':
            return string();
    }

    /* Unknown / Unicode characters skip */
    return scan_token();
}
