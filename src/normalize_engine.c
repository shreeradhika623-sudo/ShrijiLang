#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdio.h>

#include "../include/normalize_engine.h"

#define MAX_WORDS 50
#define MAX_WORD_LEN 64

/* ================================
   BASIC UTILS
================================ */

static void to_lowercase(char *s)
{
    for (int i = 0; s[i]; i++)
        s[i] = tolower(s[i]);
}

static void trim_spaces(char *s)
{
    int i = 0, j = 0;

    while (isspace(s[i])) i++;

    while (s[i])
    {
        if (!(isspace(s[i]) && isspace(s[i+1])))
            s[j++] = s[i];
        i++;
    }

    if (j > 0 && isspace(s[j-1])) j--;
    s[j] = '\0';
}

/* ================================
   TYPO DICTIONARY
================================ */

typedef struct {
    const char *wrong;
    const char *correct;
} TypoRule;

static TypoRule rules[] = {
    {"reeor", "error"},
    {"eror", "error"},
    {"softwere", "software"},
    {"creat", "create"},
    {"keise", "kaise"},
    {"helo", "hello"},
    {"hy", "hi"},
    {"sotwere", "software"},
    {"cteate", "create"},
    {"by", "bye"},

};

static int rule_count = sizeof(rules)/sizeof(TypoRule);

/* ================================
   WORD SPLIT
================================ */

static void split_words(char *input, char words[][MAX_WORD_LEN], int *count)
{
    char *token = strtok(input, " ");
    int i = 0;

    while (token && i < MAX_WORDS)
    {
        strncpy(words[i], token, MAX_WORD_LEN - 1);
        words[i][MAX_WORD_LEN - 1] = '\0';
        i++;
        token = strtok(NULL, " ");
    }

    *count = i;
}

/* ================================
   APPLY TYPO RULES
================================ */

static void apply_typo_rules(char words[][MAX_WORD_LEN], int count)
{
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < rule_count; j++)
        {
            if (strcmp(words[i], rules[j].wrong) == 0)
            {
                strncpy(words[i], rules[j].correct, MAX_WORD_LEN - 1);
            }
        }
    }
}

/* ================================
   JOIN WORDS BACK
================================ */

static void join_words(char *output, char words[][MAX_WORD_LEN], int count)
{
    output[0] = '\0';

    for (int i = 0; i < count; i++)
    {
        strncat(output, words[i], MAX_WORD_LEN - 1);
        if (i < count - 1)
            strncat(output, " ", 1);
    }
}

/* ================================
   SPECIAL CASE: EXIT NIRMAN
================================ */

static void fix_joined_commands(char *input)
{
    if (strstr(input, "exitnirman"))
    {
        strcpy(input, "exit nirman");
    }
}

/* ================================
   MAIN FUNCTION
================================ */
void normalize_input(char *input)
{

    if (!input || !*input)
        return;

    char temp[512];
    strncpy(temp, input, sizeof(temp)-1);
    temp[sizeof(temp)-1] = '\0';

    to_lowercase(temp);
    trim_spaces(temp);

    fix_joined_commands(temp);

    char words[MAX_WORDS][MAX_WORD_LEN];
    int count = 0;

    split_words(temp, words, &count);
    apply_typo_rules(words, count);

char result[512];
join_words(result, words, count);

/* safe copy back */
snprintf(input, 512, "%s", result);
}
