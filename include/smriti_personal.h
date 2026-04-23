#ifndef SMRITI_PERSONAL_H
#define SMRITI_PERSONAL_H

/* ===============================
   PERSONAL SMRITI API
   =============================== */

/* load from file (startup) */
void smriti_personal_load(void);

/* save one key */
void smriti_personal_set(const char *key, const char *value);

/* get value */
const char* smriti_personal_get(const char *key);

/* clear memory (RAM only) */
void smriti_personal_clear(void);

/* force save to file */
void smriti_personal_save(void);

#endif
