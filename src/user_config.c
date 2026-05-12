#include "../include/user_config.h"

/* ===== GLOBAL CONFIG ===== */

/* 🌸 DEV MODE */
int DEV_MODE = 0;  // 0 = USER | 1 = DEV

/* 🌸 LANGUAGE MODE */
int CURRENT_MODE = MODE_INDIA;

/* 🌸 PERSONALITY */
int CURRENT_PERSONALITY = P_SAKHI;

/* ===== INIT ===== */
void config_init()
{
    /* future config load */
}

/* ===== SETTERS ===== */
void config_set_personality(int p)
{
    CURRENT_PERSONALITY = p;
}

void config_set_mode(int mode)
{
    CURRENT_MODE = mode;
}
