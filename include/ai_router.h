#ifndef SHRIJILANG_AI_ROUTER_H
#define SHRIJILANG_AI_ROUTER_H

/*──────────────────────────────────────────────────────────────
  SHRIJILANG AI ROUTER — PUBLIC INTERFACE (L3 LOCKED)

  PURPOSE:
  - Route HUMAN CHAT messages to AI modules
  - Decide "WHO SPEAKS" (emotion / logic / knowledge / creativity)

  ABSOLUTE LAWS (DO NOT BREAK):
  1. AI Router NEVER talks to Shiri
  2. AI Router NEVER executes language logic
  3. Interpreter ONLY sends text → ai_router
  4. Execution output ALWAYS comes from Interpreter/Shiri

  This file defines ONLY routing contracts.
──────────────────────────────────────────────────────────────*/


/*──────────────────────────────────────────────
  ROUTER ENTRY POINT
──────────────────────────────────────────────*/

/*
  Called by interpreter when user types:
    sakhi "...", niyu "...", mira "...", kavya "..."

  This function handles ONLY chat / conversation.
*/
void ai_router_process(const char *message);


/*──────────────────────────────────────────────
  L3 — WHO SPEAKS RULE (ROUTING HINT ONLY)
──────────────────────────────────────────────*/

/*
  NOTE (VERY IMPORTANT):
  - This enum is a *hint*, not a command
  - It MUST NOT change interpreter behavior
  - It MUST NOT affect execution results
  - Currently used as a FUTURE HOOK only
*/

typedef enum {
    AI_ROUTER_MODE_AUTO = 0,   // Router decides automatically
    AI_ROUTER_MODE_LOGIC,      // Prefer reasoning (Niyu)
    AI_ROUTER_MODE_EMOTION,    // Prefer emotion/healing (Sakhi)
    AI_ROUTER_MODE_GUIDANCE,   // Prefer explanation/help (Mira)
    AI_ROUTER_MODE_CREATIVE    // Prefer creativity (Kavya)
} AIRouterMode;


/*──────────────────────────────────────────────
  MODE CONTROL (HOOK ONLY — NO BEHAVIOR CHANGE)
──────────────────────────────────────────────*/

/*
  Sets preferred routing mode.
  (Currently does NOT alter execution logic)
*/
void ai_router_set_mode(AIRouterMode mode);

/*
  Returns current routing preference.
*/
AIRouterMode ai_router_get_mode(void);

/*──────────────────────────────────────────────
  L3 — C → SHRIJI BRIDGE PACKET (FINAL)
  C sirf data bhejega, logic nahi
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
    const char *text;        // user ka raw sentence
    ShrijiLangHint lang;     // hint only (AUTO default)
    ShrijiSource source;     // file ya repl
} ShrijiBridgePacket;

/* NEW ROUTER ENTRY (L3) */
void ai_router_process_packet(const ShrijiBridgePacket *packet);

#endif /* SHRIJILANG_AI_ROUTER_H */
