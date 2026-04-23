#ifndef SMRITI_SESSION_H
#define SMRITI_SESSION_H

/* ===============================
   SESSION MEMORY (SHORT TERM)
   =============================== */

#include "value.h"

/* last input */
void smriti_session_set_last_input(const char *text);
const char* smriti_session_get_last_input(void);

/* last output */
void smriti_session_set_last_output(const char *text);
const char* smriti_session_get_last_output(void);

/* last evaluated value (UPGRADED) */
void smriti_session_set_last(Value v);
Value smriti_session_get_last(void);

/* clear session */
void smriti_session_clear(void);

#endif
