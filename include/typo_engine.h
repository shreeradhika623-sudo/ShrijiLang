#ifndef SHRIJI_TYPO_ENGINE_H
#define SHRIJI_TYPO_ENGINE_H

/*
    Phase-1 Typo Suggestion Engine
    Only checks for "exit" typo
*/

int shriji_check_typo(
    const char *input,
    char *suggestion,
    int suggestion_size
);

#endif
