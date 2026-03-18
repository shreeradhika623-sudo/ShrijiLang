#include "../include/smriti.h"
#include <string.h>

/* ------------------------------
   INTERNAL STATE (SILENT)
   ------------------------------ */

/* continuation (existing) */
static int    has_cont = 0;
static double cont_val = 0.0;
static char   cont_op  = 0;

/* STEP-11 context */
static unsigned long turn_counter = 0;
static char last_input_buf[512];
static int  last_intent_val = 0;

/* ------------------------------
   LIFECYCLE
   ------------------------------ */
void smriti_init(void)
{
    has_cont = 0;
    cont_val = 0.0;
    cont_op  = 0;

    turn_counter = 0;
    last_input_buf[0] = '\0';
    last_intent_val = 0;
}

/* ------------------------------
   CONTINUATION (AS-IS)
   ------------------------------ */
int smriti_has_continuation(void)
{
    return has_cont;
}

void smriti_set_continuation(double value, char op)
{
    has_cont = 1;
    cont_val = value;
    cont_op  = op;
}

void smriti_clear_continuation(void)
{
    has_cont = 0;
    cont_val = 0.0;
    cont_op  = 0;
}

double smriti_get_continuation_value(void)
{
    return cont_val;
}

char smriti_get_continuation_op(void)
{
    return cont_op;
}

/* ------------------------------
   STEP-11 CONTEXT (PASSIVE)
   ------------------------------ */
void smriti_next_turn(void)
{
    turn_counter++;
}

unsigned long smriti_get_turn(void)
{
    return turn_counter;
}

void smriti_set_last_input(const char *text)
{
    if (!text)
    {
        last_input_buf[0] = '\0';
        return;
    }
    /* safe snapshot */
    strncpy(last_input_buf, text, sizeof(last_input_buf) - 1);
    last_input_buf[sizeof(last_input_buf) - 1] = '\0';
}

const char* smriti_get_last_input(void)
{
    return last_input_buf;
}

void smriti_set_last_intent(int intent)
{
    last_intent_val = intent;
}

int smriti_get_last_intent(void)
{
    return last_intent_val;
}
