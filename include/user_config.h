#ifndef USER_CONFIG_H
#define USER_CONFIG_H

// Personality mapping
#define P_SAKHI 1
#define P_NIYU  2
#define P_MIRA  3
#define P_KAVYA 4
#define P_SHIRI 5
#define P_ALL   6   // Unified AI

// Developer mode flag (ON by default)
extern int DEV_MODE;

// Current personality (default = Sakhi)
extern int CURRENT_PERSONALITY;

void config_init();
void config_set_personality(int p);

#endif
