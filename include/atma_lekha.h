#ifndef SHRIJILANG_ATMA_LEKHA_H
#define SHRIJILANG_ATMA_LEKHA_H

/*──────────────────────────────────────────────────────────────
  ATMA-LEKHA — SYSTEM SELF AUDIT (L3-C3)

  Purpose:
  - Record system-side failures & misrouting
  - NO user data
  - Silent by default
  - No behavior change

  This is NOT Smriti.
──────────────────────────────────────────────────────────────*/

typedef enum {
    ATMA_ROUTER_FALLBACK = 1,
    ATMA_INTENT_UNKNOWN,
    ATMA_COMMAND_UNKNOWN,
    ATMA_INTERNAL_MISROUTE
} AtmaEventType;

/* initialize system self-audit */
void atma_lekha_init(void);

/* record an event (silent) */
void atma_lekha_note(AtmaEventType type,
                     const char *component,
                     const char *detail);

#endif
