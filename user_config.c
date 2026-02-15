#include "user_config.h"
#include <stdio.h>

// Developer Mode ON by Default
int DEV_MODE = 1;  // 1 = developer mode, 0 = user mode

// Default personality = Sakhi
int CURRENT_PERSONALITY = P_SAKHI;

// Default mode = INDIA
int CURRENT_MODE = MODE_INDIA;

void config_init() {
    DEV_MODE = 1;
    CURRENT_PERSONALITY = P_SAKHI;
    CURRENT_MODE = MODE_INDIA;
}

void config_set_personality(int p) {
    CURRENT_PERSONALITY = p;

    const char *name =
        (p == P_SAKHI ? "Sakhi" :
         p == P_NIYU  ? "Niyu" :
         p == P_MIRA  ? "Mira" :
         p == P_KAVYA ? "Kavya" :
         p == P_SHIRI ? "Shiri" :
         "Unified");

    printf("[Config⚙️] Personality switched to: %s\n", name);
}

void config_set_mode(int mode) {
    CURRENT_MODE = mode;

    const char *name =
        (mode == MODE_INDIA  ? "INDIA (Hindi-English mix)" :
         mode == MODE_GLOBAL ? "GLOBAL (English only)" :
         "UNKNOWN");

    printf("[Config⚙️] Mode set: %s\n", name);
}
