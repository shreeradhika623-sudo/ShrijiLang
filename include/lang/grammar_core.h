#ifndef SHRIJI_LANG_GRAMMAR_CORE_H
#define SHRIJI_LANG_GRAMMAR_CORE_H

#include "../token.h"

/*
 * Phase-1 Grammar Core
 * Only classification — no parsing logic
 */

typedef enum {
    GRAMMAR_STATEMENT,
    GRAMMAR_EXPRESSION,
    GRAMMAR_OPERATOR,
    GRAMMAR_LITERAL,

    GRAMMAR_INCDEC,     /* ++ -- */

    GRAMMAR_RESERVED,
    GRAMMAR_UNKNOWN
} GrammarKind;

/* Query API */
GrammarKind shriji_grammar_kind(TokenType t);
const char *shriji_grammar_kind_name(GrammarKind k);

#endif /* SHRIJI_LANG_GRAMMAR_CORE_H */
