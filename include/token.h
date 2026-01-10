#ifndef TOKEN_H
#define TOKEN_H

/*──────────────────────────────────────────────
  SHRIJILANG — TOKEN SYSTEM (LOCKED)
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
    TOKEN_JABTAK,
    TOKEN_NAHI,

    /* Operators */
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_MOD,

    TOKEN_EQUAL,     // =
    TOKEN_EQEQ,      // ==
    TOKEN_NEQ,       // !=
    TOKEN_GT,        // >
    TOKEN_LT,        // <
    TOKEN_GTE,       // >=
    TOKEN_LTE,       // <=

    /* Punctuation */
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_NEWLINE,   // \n
    /* Special */
    TOKEN_ERROR

} TokenType;


typedef struct {
    TokenType type;
    const char *start;
    int length;
} Token;

/* Scanner API */
void init_tokenizer(const char *source);
Token scan_token(void);

#endif

