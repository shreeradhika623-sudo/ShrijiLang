#include <string.h>
#include "../include/typo_engine.h"

/*
    Simple exit typo detection

    Rule:
    - Length must be 4
    - At least 3 characters match with "exit"
*/

int shriji_check_typo(
    const char *input,
    char *suggestion,
    int suggestion_size
)
{
    if (!input || !suggestion)
        return 0;

    const char *target = "exit";

    if (strlen(input) != 4)
        return 0;

    int match = 0;

    for (int i = 0; i < 4; i++) {
        if (input[i] == target[i])
            match++;
    }

    if (match >= 3) {
        strncpy(suggestion, target, suggestion_size - 1);
        suggestion[suggestion_size - 1] = '\0';
        return 1;
    }

    return 0;
}
