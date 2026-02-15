#include "niyu.h"
#include <string.h>

/*──────────────────────────────────────────────
 | NIYU — LOGIC ENGINE (L3 CLEAN)
 |
 | Role:
 |   • classify logic tone of user message
 |   • store last logic label for other layers
 |
 | Rules (L3):
 |   • NO printf
 |   • NO NIYU_LOG
 |   • ONLY internal state update
 *──────────────────────────────────────────────*/

/* Last logic classification (global state) */
static char last_logic_label[64] = "neutral";

/* Getter: used by niyu_bridge (pipeline) */
const char *niyu_logic_last(void)
{
    return last_logic_label;
}

/* Main logic processor (SILENT) */
void niyu_logic_process(const char *msg)
{
    char logic_type[64] = "neutral";

    /* BASIC LOGIC CLASSIFICATION */
    if (msg && *msg) {

        /* negative logic */
        if (strstr(msg, "cannot") ||
            strstr(msg, "fail")   ||
            strstr(msg, "not")) {

            strcpy(logic_type, "negative");
        }
        /* positive logic */
        else if (strstr(msg, "improve") ||
                 strstr(msg, "grow")    ||
                 strstr(msg, "succeed")) {

            strcpy(logic_type, "positive");
        }
        /* else → neutral */
    }

    /* save for pipeline use */
    strcpy(last_logic_label, logic_type);
}
