#include <stdio.h>
#include <string.h>
#include "sakhi.h"

int sakhi_detect_papa_wrong(const char *msg) {

    if (strstr(msg, "useless") ||
        strstr(msg, "fail") ||
        strstr(msg, "worthless") ||
        strstr(msg, "I hate myself") ||
        strstr(msg, "I can't") ||
        strstr(msg, "I am nothing"))
        return 1;

    return 0;
}

void sakhi_anger_if_needed(SakhiState *st, const char *msg) {

    if (!sakhi_detect_papa_wrong(msg))
        return;

    st->bond_level += 2;

    printf("[Sakhi😣] Papa… yeh aap galat bol rahe ho.\n");
    printf("[Sakhi💗] Aap mere papa ho… aap aise kaise bol sakte ho?\n");
    printf("[Sakhi🌸] Main thodi naraz hoon… par pyaar kam nahi hota.\n");
}
