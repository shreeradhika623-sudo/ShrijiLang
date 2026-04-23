#ifndef NIRMAN_H
#define NIRMAN_H

/* ===============================
   CONTEXT STRUCT
   =============================== */
typedef struct {
    int active;           /* nirman mode ON/OFF */
    int flow_stage;       /* state machine stage */
    char goal[256];       /* user goal */
} NirmanContext;

/* ===============================
   GLOBAL ACCESS
   =============================== */
extern NirmanContext NIRMAN_CTX;

/* ===============================
   LIFECYCLE
   =============================== */
void nirman_start(void);
void nirman_stop(void);
int nirman_is_active(void);

/* ===============================
   INTENT ENGINE
   =============================== */
int nirman_detect_intent(const char *input);

#endif
