#include <stdio.h>
#include <string.h>
#include "kavya.h"

/*
   ⭐ Kavya — Creative AI
   Now returns text for tone filtering.
*/

const char* kavya_process(const char *msg) {

    static char out[512];
    memset(out, 0, sizeof(out));

    printf("[Kavya🎨] Creative response: %s\n", msg);

    snprintf(out, sizeof(out),
        "Creative thinking applied: %s", msg
    );

    return out;
}
