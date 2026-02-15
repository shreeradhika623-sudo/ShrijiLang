#ifndef TRANSLATOR_H
#define TRANSLATOR_H

// Translate English/world command → ShrijiLang keyword
const char* translate_keyword(const char *word);

// Autocorrect system for mistyped user commands
const char* mira_autocorrect(const char *word);

// Extract first word from input (helper for parser)
char* get_first_word(const char *source);

#endif
