#include "../../../include/lang/grammar_core.h"

/*
 * Phase-2 Grammar Core
 * Classification only (no parsing / execution)
 */

GrammarKind shriji_grammar_kind(TokenType t)
{
    switch (t) {

        /* ───────── STATEMENTS ───────── */
        case TOKEN_MAVI:
        case TOKEN_KAAM:
        case TOKEN_AGAR:
        case TOKEN_JABTAK:
        case TOKEN_WAPAS:
        case TOKEN_IMPORT:
        case TOKEN_WARNA:
        case TOKEN_RUKJA:
        case TOKEN_CHALU:
            return GRAMMAR_STATEMENT;

        /* ───────── LITERALS ───────── */
        case TOKEN_NUMBER:
        case TOKEN_STRING:
        case TOKEN_TRUE:
        case TOKEN_FALSE:
            return GRAMMAR_LITERAL;

        /* ───────── INC / DEC ───────── */
        case TOKEN_PLUSPLUS:
        case TOKEN_MINUSMINUS:
            return GRAMMAR_INCDEC;

        /* ───────── LOGICAL / ARITHMETIC OPERATORS ───────── */
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_STAR:
        case TOKEN_SLASH:
        case TOKEN_MOD:

        case TOKEN_EQEQ:
        case TOKEN_NEQ:
        case TOKEN_GT:
        case TOKEN_LT:
        case TOKEN_GTE:
        case TOKEN_LTE:

        case TOKEN_AND:
        case TOKEN_OR:
        case TOKEN_NOT:
            return GRAMMAR_OPERATOR;

        default:
            return GRAMMAR_UNKNOWN;
    }
}

const char *shriji_grammar_kind_name(GrammarKind k)
{
    switch (k) {
        case GRAMMAR_STATEMENT:  return "STATEMENT";
        case GRAMMAR_EXPRESSION: return "EXPRESSION";
        case GRAMMAR_OPERATOR:   return "OPERATOR";
        case GRAMMAR_LITERAL:    return "LITERAL";
        case GRAMMAR_INCDEC:     return "INCDEC";
        case GRAMMAR_RESERVED:   return "RESERVED";
        default:                 return "UNKNOWN";
    }
}
