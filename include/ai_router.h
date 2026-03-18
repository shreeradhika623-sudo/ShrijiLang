#ifndef SHRIJILANG_AI_ROUTER_H
#define SHRIJILANG_AI_ROUTER_H

#include "../pragya/contracts/l3_response.h"

/*──────────────────────────────────────────────
  SHRIJILANG AI ROUTER — PUBLIC INTERFACE (L3)
──────────────────────────────────────────────*/

typedef enum {
    SHRIJI_LANG_AUTO = 0,
    SHRIJI_LANG_HINDI_MIX,
    SHRIJI_LANG_ENGLISH
} ShrijiLangHint;

typedef enum {
    SHRIJI_SRC_FILE = 0,
    SHRIJI_SRC_REPL
} ShrijiSource;

typedef struct {
    const char      *text;
    ShrijiLangHint   lang;
    ShrijiSource     source;
} ShrijiBridgePacket;

/* NEW: L3 dispatcher */
L3Response ai_router_dispatch(const ShrijiBridgePacket *packet);

/* old compatibility */
void ai_router_process(const char *message);

#endif /* SHRIJILANG_AI_ROUTER_H */
