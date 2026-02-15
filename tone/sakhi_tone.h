#ifndef SAKHI_TONE_H
#define SAKHI_TONE_H

/*──────────────────────────────────────────────
  SAKHI TONE FILTER — L3
  ---------------------------------------------
  ROLE:
  - Sirf language soft / neutral banana
  - Koi naya emotion add NAHI karta
  - Koi relationship / ownership words NAHI
  - Output deterministic & safe
──────────────────────────────────────────────*/

/*
  Input  : raw decided text (usually from Niyu)
  Output : tone-adjusted text (no semantic change)
*/
const char* sakhi_tone_filter(const char* raw);

#endif /* SAKHI_TONE_H */
