#include "../../include/lang/grammar.h"
#include "../../include/token.h"

/*
 * SHRIJILANG GRAMMAR CORE
 * Phase-1: token presence validation only
 * No parser logic change yet
 */

void shriji_register_grammar_core(void)
{
    /*
     * IMPORTANT:
     * This function is called once from parse_program()
     * Do NOT put parser logic here.
     * Only grammar / token registration / validation.
     */

#ifdef SHRIJI_ENABLE_MASTER_TOKENS
    /* Phase-1: sanity check hook */
    /* (empty by design for now) */
#endif
}

/* Future phases — intentionally empty */

void shriji_register_grammar_logic(void) {}
void shriji_register_grammar_incdec(void) {}
void shriji_register_grammar_loop(void) {}
void shriji_register_grammar_func(void) {}
