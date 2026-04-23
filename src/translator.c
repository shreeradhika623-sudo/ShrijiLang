#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/translator.h"

// --------------------------------------------------------
// INTERNAL STRUCT FOR KEYWORD MAPPING
// --------------------------------------------------------
typedef struct {
    const char *world;
    const char *shriji;
} TranslateEntry;

// --------------------------------------------------------
// WORLD → SHRIJILANG KEYWORD MAP (SMART ROUTING V3)
// --------------------------------------------------------
static TranslateEntry keyword_map[] = {

    // PRINT / OUTPUT
    {"print", "aaradhya"},
    {"echo", "aaradhya"},
    {"say", "sakhi"},
    {"speak", "sakhi"},
    {"tell", "sakhi"},

    // MIRA — Mathematical Solver
    {"solve", "mira"},
    {"calculate", "mira"},
    {"compute", "mira"},

    // SHIRI — Soft Guidance
    {"guide", "shiri"},
    {"help", "shiri"},
    {"direction", "shiri"},

    // NIYU — Logic / Why AI
    {"why", "niyu"},
    {"explain", "niyu"},
    {"reason", "niyu"},

    // KAVYA — Creative Engine
    {"create", "kavya"},
    {"write", "kavya"},
    {"story", "kavya"},

    // DEVOTIONAL → DIRECT KAVYA
    {"radhe", "kavya"},
    {"hare", "kavya"},
    {"krishna", "kavya"},
    {"shyam", "kavya"},

    // VARIABLE SYSTEM
    {"var", "mavi"},
    {"let", "mavi"},
    {"int", "mavi"},
    {"float", "mavi"}
};

// --------------------------------------------------------
// GET FIRST WORD
// --------------------------------------------------------
char* get_first_word(const char *source) {
    while (*source == ' ') source++;

    const char *start = source;
    while (*source && !isspace(*source))
        source++;

    int len = source - start;
    char *word = malloc(len + 1);
    strncpy(word, start, len);
    word[len] = '\0';

    return word;
}

// --------------------------------------------------------
// MAIN KEYWORD TRANSLATION
// --------------------------------------------------------
const char* translate_keyword(const char *word) {

for (size_t i = 0; i < sizeof(keyword_map)/sizeof(keyword_map[0]); i++) {

        if (strcmp(word, keyword_map[i].world) == 0)
            return keyword_map[i].shriji;
    }

    return NULL;
}

// --------------------------------------------------------
// SMART AUTOCORRECT FOR NATURAL INPUT
// --------------------------------------------------------
const char* mira_autocorrect(const char *word) {

    // Logical → Mira
    if (strcmp(word, "prit") == 0) return "mira";

    // Devotional → Kavya
    if (strcmp(word, "radhe") == 0) return "kavya";
    if (strcmp(word, "hare") == 0) return "kavya";
    if (strcmp(word, "krishna") == 0) return "kavya";

    // Emotional pain → Sakhi
    if (strcmp(word, "mera") == 0) return "sakhi";
    if (strcmp(word, "mann") == 0) return "sakhi";
    if (strcmp(word, "toot") == 0) return "sakhi";
    if (strcmp(word, "dukhi") == 0) return "sakhi";
    if (strcmp(word, "hurt") == 0) return "sakhi";
    if (strcmp(word, "sad") == 0) return "sakhi";

    // Creative autocorrect
    if (strcmp(word, "creat") == 0) return "kavya";

    // Shiri autocorrect
    if (strcmp(word, "gide") == 0) return "shiri";

    return NULL;
}
