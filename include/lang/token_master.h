#ifndef SHRIJI_TOKEN_MASTER_H
#define SHRIJI_TOKEN_MASTER_H

/*
  SHRIJILANG — MASTER TOKEN REGISTRY
  ---------------------------------
  This file is a DESIGN + FUTURE registry.

  RULES:
  1. token.h is the runtime truth
  2. Only tokens present in token.h are ACTIVE
  3. Others are FUTURE / PARKED (no runtime meaning yet)
*/

/* =====================================================
   CORE / ACTIVE TOKENS (MIRROR OF token.h)
   ===================================================== */

typedef enum {

    /* ----- Special ----- */
    MT_TOKEN_EOF = 0,
    MT_TOKEN_ERROR,
    MT_TOKEN_NEWLINE,

    /* ----- Literals ----- */
    MT_TOKEN_NUMBER,
    MT_TOKEN_IDENTIFIER,
    MT_TOKEN_STRING,
    MT_TOKEN_TRUE,
    MT_TOKEN_FALSE,

    /* ----- Core Keywords ----- */
    MT_TOKEN_MAVI,
    MT_TOKEN_AGAR,
    MT_TOKEN_WARNA,
    MT_TOKEN_KAAM,
    MT_TOKEN_JABTAK,
    MT_TOKEN_RUKJA,
    MT_TOKEN_CHALU,
    MT_TOKEN_NAHI,
    MT_TOKEN_BOLO,
    MT_TOKEN_WAPAS,
    MT_TOKEN_IMPORT,

    /* ----- AI / System Commands ----- */
    MT_TOKEN_SAKHI,
    MT_TOKEN_NIYU,
    MT_TOKEN_MIRA,
    MT_TOKEN_KAVYA,
    MT_TOKEN_SHIRI,
    MT_TOKEN_SAMAY,

    /* ----- Operators ----- */
    MT_TOKEN_PLUS,
    MT_TOKEN_MINUS,
    MT_TOKEN_STAR,
    MT_TOKEN_SLASH,
    MT_TOKEN_MOD,

    MT_TOKEN_EQUAL,
    MT_TOKEN_EQEQ,
    MT_TOKEN_NEQ,
    MT_TOKEN_GT,
    MT_TOKEN_LT,
    MT_TOKEN_GTE,
    MT_TOKEN_LTE,

    /* ----- Increment / Decrement ----- */
    MT_TOKEN_PLUSPLUS,
    MT_TOKEN_MINUSMINUS,

    /* ----- Logical ----- */
    MT_TOKEN_AND,
    MT_TOKEN_OR,
    MT_TOKEN_NOT,

    /* ----- Punctuation ----- */
    MT_TOKEN_LEFT_PAREN,
    MT_TOKEN_RIGHT_PAREN,
    MT_TOKEN_LEFT_BRACE,
    MT_TOKEN_RIGHT_BRACE,
    MT_TOKEN_LEFT_BRACKET,
    MT_TOKEN_RIGHT_BRACKET,
    MT_TOKEN_COMMA,
    MT_TOKEN_COLON,

/* =====================================================
   FUTURE / PARKED TOKENS (NOT IN token.h YET)
   ===================================================== */

    /* ----- Declarations ----- */
    MT_TOKEN_CONST,
    MT_TOKEN_LET,

    /* ----- Types ----- */
    MT_TOKEN_TYPE_INT,
    MT_TOKEN_TYPE_FLOAT,
    MT_TOKEN_TYPE_STRING,
    MT_TOKEN_TYPE_BOOL,
    MT_TOKEN_TYPE_LIST,
    MT_TOKEN_TYPE_DICT,
    MT_TOKEN_TYPE_ANY,

    /* ----- Control Flow (future) ----- */
    MT_TOKEN_FOR,
    MT_TOKEN_DO,
    MT_TOKEN_SWITCH,
    MT_TOKEN_CASE,
    MT_TOKEN_DEFAULT,

    /* ----- Assignment Variants ----- */
    MT_TOKEN_PLUSEQ,
    MT_TOKEN_MINUSEQ,
    MT_TOKEN_STAREQ,
    MT_TOKEN_SLASHEQ,
    MT_TOKEN_MODEQ,

    /* ----- Strict / Advanced Compare ----- */
    MT_TOKEN_STRICT_EQ,
    MT_TOKEN_STRICT_NEQ,

    /* ----- Bitwise ----- */
    MT_TOKEN_BIT_AND,
    MT_TOKEN_BIT_OR,
    MT_TOKEN_BIT_XOR,
    MT_TOKEN_BIT_NOT,
    MT_TOKEN_SHIFT_LEFT,
    MT_TOKEN_SHIFT_RIGHT,

    /* ----- Ternary ----- */
    MT_TOKEN_QUESTION,

    /* ----- Structure (future sugar) ----- */
    MT_TOKEN_DOT,
    MT_TOKEN_ARROW,

    /* ----- Data Structures ----- */
    MT_TOKEN_LIST,
    MT_TOKEN_DICT,
    MT_TOKEN_SET,
    MT_TOKEN_TUPLE,

    /* ----- Sentinel ----- */
    MT_TOKEN__COUNT

} ShrijiMasterToken;

#endif /* SHRIJI_TOKEN_MASTER_H */
