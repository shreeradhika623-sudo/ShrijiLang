CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isakhi -Iniyu -Ishiri -Imira -Ikavya -Itone

SRC = \
    src/token.c \
    src/ast.c \
    src/parser.c \
    src/interpreter.c \
    src/error.c \
    src/state.c \
    src/event.c \
    src/command.c \
    env/env.c \
    ai_router.c \
    src/translator.c \
    main.c \
    \
    sakhi/sakhi_bridge.c \
    sakhi/sakhi_core.c \
    sakhi/sakhi_memory.c \
    sakhi/sakhi_emotion.c \
    sakhi/sakhi_healing.c \
    sakhi/sakhi_anger.c \
    sakhi/sakhi_predict.c \
    sakhi/sakhi_personality.c \
    sakhi/sakhi_bonding.c \
    sakhi/sakhi_states.c \
    sakhi/sakhi_context.c \
    sakhi/sakhi_intensity.c \
    sakhi/sakhi_response_engine.c \
    \
    niyu/niyu_bridge.c \
    niyu/niyu_logic.c \
    niyu/niyu_memory.c \
    niyu/niyu_emotion.c \
    niyu/niyu_observation.c \
    niyu/niyu_deduction.c \
    niyu/niyu_induction.c \
    niyu/niyu_resolve.c \
    niyu/niyu_synthesis.c \
    niyu/niyu_meta.c \
    niyu/niyu_consistency.c \
    niyu/niyu_cause.c \
    niyu/niyu_effect.c \
    niyu/niyu_clarity.c \
    niyu/niyu_self.c \
    niyu/niyu_vision.c \
    niyu/niyu_reason.c \
    niyu/niyu_explain.c \
    niyu/niyu_why_engine.c \
    niyu/niyu_root.c \
    niyu/niyu_chain.c \
    niyu/niyu_depth.c \
    niyu/niyu_stability.c \
    niyu/niyu_anchor.c \
    niyu/niyu_shield.c \
    niyu/niyu_identity.c \
    niyu/niyu_predict.c \
    niyu/niyu_meta10_esi.c \
    niyu/niyu_she.c \
    niyu/niyu_sol.c \
    niyu/niyu_mfl.c \
    \
    shiri/shiri_core.c \
    mira/mira_core.c \
    kavya/kavya_core.c \
    tone/sakhi_tone.c \
    user_config.c

OUT = shrijilang

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
	rm -f *.o
	rm -f */*.o

run: $(OUT)
	./$(OUT)
