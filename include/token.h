#ifndef TOKEN_H
#define TOKEN_H

/*──────────────────────────────────────────────
  SHRIJILANG — TOKEN SYSTEM (LOCKED)
  Now supports: line/col for professional errors
──────────────────────────────────────────────*/

typedef enum {

    TOKEN_EOF = 0,

    /* Literals */
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_TRUE,
    TOKEN_FALSE,

    /* Keywords */
    TOKEN_MAVI,
    TOKEN_AGAR,
    TOKEN_WARNA,
    TOKEN_KAAM,
    TOKEN_JABTAK,
    TOKEN_RUKJA,
    TOKEN_CHALU,
    TOKEN_NAHI,
    TOKEN_BOLO,

    TOKEN_SAKHI,
    TOKEN_NIYU,
    TOKEN_MIRA,
    TOKEN_KAVYA,
    TOKEN_SHIRI,

    TOKEN_SAMAY,
    TOKEN_WAPAS,

    TOKEN_IMPORT,
    TOKEN_RACHNA,

    /* Operators */
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_MOD,

    TOKEN_EQUAL,
    TOKEN_EQEQ,
    TOKEN_NEQ,
    TOKEN_GT,
    TOKEN_LT,
    TOKEN_GTE,
    TOKEN_LTE,

/* Increment / Decrement */
TOKEN_PLUSPLUS,     // ++
TOKEN_MINUSMINUS,   // --

/* Logical Operators */
TOKEN_AND,          // &&
TOKEN_OR,           // ||
TOKEN_NOT,          // !

    /* Punctuation */
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_NEWLINE,

    /* Special */
    TOKEN_ERROR

} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    int length;
    int line;
    int col;
} Token;

/* Scanner API */
void init_tokenizer(const char *source);
Token scan_token(void);

//* #ifdef SHRIJI_ENABLE_MASTER_TOKENS
//* #include "lang/token_master.h"
//* #endif

#endif // TOKEN_H
