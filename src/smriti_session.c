#include "../include/smriti_session.h"
#include <string.h>
#include <stdlib.h>

/* ===============================
   INTERNAL STORAGE
   =============================== */

static char last_input[256] = {0};
static char last_output[256] = {0};

static Value last_value;
static int has_last = 0;

/* ===============================
   LAST INPUT
   =============================== */

void smriti_session_set_last_input(const char *text)
{
    if (!text) return;

    strncpy(last_input, text, sizeof(last_input) - 1);
    last_input[sizeof(last_input) - 1] = '\0';
}

const char* smriti_session_get_last_input(void)
{
    return last_input;
}

/* ===============================
   LAST OUTPUT
   =============================== */

void smriti_session_set_last_output(const char *text)
{
    if (!text) return;

    strncpy(last_output, text, sizeof(last_output) - 1);
    last_output[sizeof(last_output) - 1] = '\0';
}

const char* smriti_session_get_last_output(void)
{
    return last_output;
}

/* ===============================
   LAST VALUE (UPGRADED)
   =============================== */

void smriti_session_set_last(Value v)
{
    if (has_last)
        value_free(&last_value);

    last_value = value_copy(v);
    has_last = 1;
}

Value smriti_session_get_last(void)
{
    if (!has_last)
        return value_null();

    return value_copy(last_value);
}

/* ===============================
   CLEAR SESSION
   =============================== */

void smriti_session_clear(void)
{
    last_input[0] = '\0';
    last_output[0] = '\0';

    if (has_last)
        value_free(&last_value);

    has_last = 0;
}
