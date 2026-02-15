#ifndef SAKHI_H
#define SAKHI_H

/*──────────────────────────────────────────────
  SAKHI — L3 SPEAK ONLY MODULE
  ---------------------------------------------
  RULES (LOCKED):
  1. Sakhi NEVER thinks
  2. Sakhi NEVER decides
  3. Sakhi ONLY speaks what is given
  4. No emotional ownership
  5. No relationship / dependency language
──────────────────────────────────────────────*/

/*
  SakhiInput:
  - already processed / decided text
  - typically comes from Niyu via ai_router
*/
typedef struct {
    const char *text;      /* final content to speak */
} SakhiInput;

/*
  SakhiOutput:
  - final safe string to print
*/
typedef struct {
    const char *spoken;   /* sanitized + toned output */
} SakhiOutput;

/*
  Main public API
  ----------------
  Input  : SakhiInput*
  Output : SakhiOutput
*/
SakhiOutput sakhi_speak(const SakhiInput *in);

#endif /* SAKHI_H */
