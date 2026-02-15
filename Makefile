CC = gcc

CFLAGS = -Wall -Wextra \
	-Iinclude \
	-Isrc \
	-Ienv

# ===============================
# CORE LANGUAGE (L1 + L2)
# ===============================

SRC = \
	src/token.c \
	src/ast.c \
	src/parser.c \
	src/interpreter.c \
	src/error.c \
	src/state.c \
	src/event.c \
	src/command.c \
	src/translator.c \
	src/ai_router.c \
	src/ai_intent.c \
	src/atma_lekha.c \
        src/smriti.c \
	src/main.c \
	\
	env/env.c \
	\
	src/lang/grammar/grammar_registry.c \
	src/lang/grammar/grammar_core.c

# ===============================
# OUTPUT
# ===============================

OUT = shrijilang

# ===============================
# BUILD RULES
# ===============================

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
	rm -f *.o
	rm -f src/*.o
	rm -f env/*.o
	rm -f src/lang/grammar/*.o

run: $(OUT)
	./$(OUT)
