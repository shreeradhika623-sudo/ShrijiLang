#ifndef SHRIJI_LANG_GRAMMAR_H
#define SHRIJI_LANG_GRAMMAR_H

/* Grammar extension registry
 * Phase-1: only hooks, no grammar rules
 */

/* Called once during parser init */
void shriji_register_grammar_core(void);

/* Optional future grammars */
void shriji_register_grammar_logic(void);
void shriji_register_grammar_incdec(void);
void shriji_register_grammar_loop(void);
void shriji_register_grammar_func(void);

#endif /* SHRIJI_LANG_GRAMMAR_H */
