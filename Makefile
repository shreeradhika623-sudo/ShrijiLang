# ============================================================
# SHRIJILANG — UNIFIED BUILD SYSTEM (KRST + PRAGYA)
# ============================================================

CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -O2 \
	-Iinclude \
	-Isrc \
	-Ienv \
	-Ipragya \
	-Ikrst

# ============================================================
# CORE SOURCE FILES
# ============================================================

SRC = \
	src/token.c \
	src/ast.c \
	src/parser.c \
	gyaan/core/gyaan_engine.c \
	gyaan/data/gyaan_rules.c \
	src/interpreter.c \
	src/runtime.c \
	src/error.c \
	src/state.c \
	src/event.c \
	src/command.c \
	src/translator.c \
	src/example_builder.c \
	src/ai_router.c \
	src/ai_intent.c \
	src/atma_lekha.c \
	src/smriti.c \
        src/smriti_session.c \
	src/smriti_personal.c \
	src/main.c \
	env/env.c \
	src/lang/grammar/grammar_registry.c \
	src/lang/grammar/grammar_core.c \
	pragya/router/l3_router.c \
	pragya/niyu/niyu_core.c \
	pragya/sakhi/sakhi_core.c \
	pragya/mira/mira_core.c \
	krst/krst_core.c \
	krst/krst_router.c \
	krst/niyu_engine.c \
	src/fix_engine.c \
	src/fix_rules.c \
	src/fix_interactive.c \
	src/typo_engine.c \
	src/error_intelligence.c \
	src/decision_engine.c \
src/user_config.c \
src/nirman.c \
src/normalize_engine.c \
src/nirman_flow_engine.c \

# ============================================================
# OUTPUT
# ============================================================

OUT = shrijilang

# ============================================================
# BUILD RULES
# ============================================================

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) -lreadline

clean:
	rm -f $(OUT)
	rm -f *.o
	rm -f src/*.o
	rm -f env/*.o
	rm -f src/lang/grammar/*.o
	rm -f pragya/router/*.o
	rm -f pragya/niyu/*.o
	rm -f pragya/sakhi/*.o
	rm -f pragya/mira/*.o
	rm -f krst/*.o

run: $(OUT)
	./$(OUT)

rebuild: clean $(OUT)
