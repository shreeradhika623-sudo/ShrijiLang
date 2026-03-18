#ifndef SMRITI_H
#define SMRITI_H

/* lifecycle */
void smriti_init(void);

/* math continuation (existing behavior) */
int  smriti_has_continuation(void);
void smriti_set_continuation(double value, char op);
void smriti_clear_continuation(void);

double smriti_get_continuation_value(void);
char   smriti_get_continuation_op(void);

/* ===== STEP-11: context (NO logic, NO decision) ===== */

/* turn tracking */
void smriti_next_turn(void);
unsigned long smriti_get_turn(void);

/* last input snapshot */
void smriti_set_last_input(const char *text);
const char* smriti_get_last_input(void);

/* last intent snapshot (L3 intent enum value passed as int) */
void smriti_set_last_intent(int intent);
int  smriti_get_last_intent(void);

#endif
