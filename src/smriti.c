#include "../include/smriti.h"
#include <time.h>

/*──────────────────────────────────────────────────────────────
  GLOBAL SESSION MEMORY (RAM ONLY)
  (PURE STATE — NO CONTROL, NO DECISION)
──────────────────────────────────────────────────────────────*/
SessionSmriti SESSION_SMRITI;

/*──────────────────────────────────────────────────────────────
  Initialize Smriti (SESSION START)
──────────────────────────────────────────────────────────────*/
void smriti_init(void)
{
    SESSION_SMRITI.session_start = time(NULL);

    SESSION_SMRITI.repeat_requested = 0;
    SESSION_SMRITI.clarification_requested = 0;
}

/*──────────────────────────────────────────────────────────────
  Reset Smriti (same run ke andar)
──────────────────────────────────────────────────────────────*/
void smriti_reset(void)
{
    smriti_init();
}

/*──────────────────────────────────────────────────────────────
  Context Flags — REPEAT
  (NON-DECISIVE, USER-VISIBLE)
──────────────────────────────────────────────────────────────*/
void smriti_set_repeat_requested(int on)
{
    SESSION_SMRITI.repeat_requested = (on ? 1 : 0);
}

int smriti_is_repeat_requested(void)
{
    return SESSION_SMRITI.repeat_requested;
}

/*──────────────────────────────────────────────────────────────
  Context Flags — CLARIFICATION
  (NON-DECISIVE, USER-VISIBLE)
──────────────────────────────────────────────────────────────*/
void smriti_set_clarification_requested(int on)
{
    SESSION_SMRITI.clarification_requested = (on ? 1 : 0);
}

int smriti_is_clarification_requested(void)
{
    return SESSION_SMRITI.clarification_requested;
}
