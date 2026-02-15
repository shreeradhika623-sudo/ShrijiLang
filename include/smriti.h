#ifndef SMRITI_H
#define SMRITI_H

#include <time.h>

/*──────────────────────────────────────────────────────────────
  SMRITI — L3 SESSION MEMORY (FROZEN)
  
  DESIGN LAW (LOCKED):
  1. RAM only (session memory)
  2. Program exit par destroy
  3. Disk / file ka use NAHI
  4. Koi decision / control NAHI
  5. Sirf context & recall ke liye
──────────────────────────────────────────────────────────────*/

/*──────────────────────────────────────────────────────────────
  SESSION MEMORY STRUCTURE
  (PURE STATE — NO LOGIC)
──────────────────────────────────────────────────────────────*/
typedef struct {

    /* Session meta */
    time_t session_start;

    /* Context flags (NON-DECISIVE) */
    int repeat_requested;          /* user ne repeat bola */
    int clarification_requested;   /* user ne clarify bola */

} SessionSmriti;

/*──────────────────────────────────────────────────────────────
  GLOBAL SESSION INSTANCE (RAM ONLY)
──────────────────────────────────────────────────────────────*/
extern SessionSmriti SESSION_SMRITI;

/*──────────────────────────────────────────────────────────────
  API — MEMORY ONLY
──────────────────────────────────────────────────────────────*/

/* lifecycle */
void smriti_init(void);
void smriti_reset(void);

/* context flags */
void smriti_set_repeat_requested(int on);
int  smriti_is_repeat_requested(void);

void smriti_set_clarification_requested(int on);
int  smriti_is_clarification_requested(void);

#endif /* SMRITI_H */
