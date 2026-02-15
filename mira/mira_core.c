#include <string.h>
#include <stdio.h>

#include "../include/user_config.h"

/* -------------------------------------------------------
 * Mira Output Helper (Mode aware)
 * ------------------------------------------------------- */
static void mira_say(char *out, size_t outsz, const char *india, const char *global)
{
    if (CURRENT_MODE == MODE_GLOBAL) {
        snprintf(out, outsz, "%s", global);
    } else {
        snprintf(out, outsz, "%s", india);
    }
}

/* -------------------------------------------------------
 * Simple session memory (no file save yet)
 * ------------------------------------------------------- */
static int mira_last_lesson = 0;

/* -------------------------------------------------------
 * Mira Test State (L3)
 * ------------------------------------------------------- */
static int mira_test_active = 0;
static int mira_test_step = 0;
static int mira_test_score = 0;

/* -------------------------------------------------------
 * Mira Teacher: Help Text
 * ------------------------------------------------------- */
static void mira_help(char *out, size_t outsz)
{
    mira_say(out, outsz,
        "[Mira📐] Mira Teacher Mode Help:\n"
        "Commands:\n"
        "  mira \"help\"       -> help dikhaye\n"
        "  mira \"start\"      -> learning start kare\n"
        "  mira \"lesson 1\"   -> intro\n"
        "  mira \"lesson 2\"   -> variables\n"
        "  mira \"next\"       -> next lesson\n"
        "  mira \"repeat\"     -> last lesson repeat\n"
        "  mira \"test 1\"     -> test start kare\n"
        "\n"
        "Tip: mode india/global se language switch hoti hai.",

        "[Mira📐] Mira Teacher Mode Help:\n"
        "Commands:\n"
        "  mira \"help\"       -> show help\n"
        "  mira \"start\"      -> start learning\n"
        "  mira \"lesson 1\"   -> intro\n"
        "  mira \"lesson 2\"   -> variables\n"
        "  mira \"next\"       -> next lesson\n"
        "  mira \"repeat\"     -> repeat last lesson\n"
        "  mira \"test 1\"     -> start test\n"
        "\n"
        "Tip: use mode india/global to switch language."
    );
}

/* -------------------------------------------------------
 * Mira Teacher: Lesson 1 (Intro)
 * ------------------------------------------------------- */
static void mira_lesson_1(char *out, size_t outsz)
{
    mira_last_lesson = 1;

    mira_say(out, outsz,
        "[Mira📐] Lesson 1: Introduction\n"
        "ShrijiLang ek scripting language hai jo beginners ke liye bana hai.\n"
        "\n"
        "Example:\n"
        "  bolo \"hello\"\n"
        "\n"
        "Try:\n"
        "  bolo \"Main ShrijiLang seekh raha hoon\"\n"
        "\n"
        "Next:\n"
        "  mira \"next\"",

        "[Mira📐] Lesson 1: Introduction\n"
        "ShrijiLang is a beginner-friendly scripting language.\n"
        "\n"
        "Example:\n"
        "  bolo \"hello\"\n"
        "\n"
        "Try:\n"
        "  bolo \"I am learning ShrijiLang\"\n"
        "\n"
        "Next:\n"
        "  mira \"next\""
    );
}

/* -------------------------------------------------------
 * Mira Teacher: Lesson 2 (Variables)
 * ------------------------------------------------------- */
static void mira_lesson_2(char *out, size_t outsz)
{
    mira_last_lesson = 2;

    mira_say(out, outsz,
        "[Mira📐] Lesson 2: Variables (mavi)\n"
        "Variable ka matlab hota hai: value store karna.\n"
        "\n"
        "Syntax:\n"
        "  mavi x = 10\n"
        "\n"
        "Example:\n"
        "  mavi age = 20\n"
        "  bolo age\n"
        "\n"
        "Update:\n"
        "  age = age + 1\n"
        "  bolo age\n"
        "\n"
        "Practice:\n"
        "  1) mavi a = 5\n"
        "  2) mavi b = 7\n"
        "  3) bolo a + b\n",

        "[Mira📐] Lesson 2: Variables (mavi)\n"
        "A variable stores a value.\n"
        "\n"
        "Syntax:\n"
        "  mavi x = 10\n"
        "\n"
        "Example:\n"
        "  mavi age = 20\n"
        "  bolo age\n"
        "\n"
        "Update:\n"
        "  age = age + 1\n"
        "  bolo age\n"
        "\n"
        "Practice:\n"
        "  1) mavi a = 5\n"
        "  2) mavi b = 7\n"
        "  3) bolo a + b\n"
    );
}

static void mira_repeat_last(char *out, size_t outsz)
{
    if (mira_last_lesson == 1) {
        mira_lesson_1(out, outsz);
        return;
    }
    if (mira_last_lesson == 2) {
        mira_lesson_2(out, outsz);
        return;
    }

    mira_say(out, outsz,
             "[Mira📐] Abhi koi lesson start nahi hua. Try: mira \"lesson 1\"",
             "[Mira📐] No lesson has started yet. Try: mira \"lesson 1\"");
}

static void mira_next(char *out, size_t outsz)
{
    if (mira_last_lesson == 0) {
        mira_lesson_1(out, outsz);
        return;
    }
    if (mira_last_lesson == 1) {
        mira_lesson_2(out, outsz);
        return;
    }

    mira_say(out, outsz,
             "[Mira📐] Abhi ke liye lessons yahin tak hain.\nTry: mira \"repeat\"",
             "[Mira📐] No more lessons yet.\nTry: mira \"repeat\"");
}

/* -------------------------------------------------------
 * Mira Test Handler (Test 1)
 * ------------------------------------------------------- */
static void mira_test_handler(char *out, size_t outsz, const char *msg)
{
    if (!mira_test_active) {
        mira_test_active = 1;
        mira_test_step = 0;
        mira_test_score = 0;

        mira_say(out, outsz,
            "[Mira📐] Test 1 start ho raha hai.\nQ1: bolo \"hello\" ka output kya hoga?",
            "[Mira📐] Test 1 starting.\nQ1: What is the output of bolo \"hello\" ?");
        return;
    }

    if (mira_test_step == 0) {
        if (strcmp(msg, "hello") == 0) {
            mira_test_score++;
            mira_say(out, outsz,
                "[Mira📐] Sahi.\nQ2: mavi x=5, mavi y=7, bolo x+y ka output?",
                "[Mira📐] Correct.\nQ2: Output of mavi x=5, mavi y=7, bolo x+y ?");
        } else {
            mira_say(out, outsz,
                "[Mira📐] Galat. Sahi answer: hello\nQ2: mavi x=5, mavi y=7, bolo x+y?",
                "[Mira📐] Wrong. Correct answer: hello\nQ2: Output of mavi x=5, mavi y=7, bolo x+y ?");
        }
        mira_test_step = 1;
        return;
    }

    if (mira_test_step == 1) {
        if (strcmp(msg, "12") == 0) {
            mira_test_score++;
            mira_say(out, outsz,
                "[Mira📐] Sahi.\nQ3: mavi age=10; age=age+1; bolo age ka output?",
                "[Mira📐] Correct.\nQ3: Output of mavi age=10; age=age+1; bolo age ?");
        } else {
            mira_say(out, outsz,
                "[Mira📐] Galat. Sahi answer: 12\nQ3: mavi age=10; age=age+1; bolo age?",
                "[Mira📐] Wrong. Correct answer: 12\nQ3: Output of mavi age=10; age=age+1; bolo age ?");
        }
        mira_test_step = 2;
        return;
    }

    if (mira_test_step == 2) {
        if (strcmp(msg, "11") == 0) {
            mira_test_score++;
        }

        snprintf(out, outsz,
            "[Mira📐] Test complete.\nScore: %d/3\nTip: lesson 2 repeat karo.",
            mira_test_score);

        mira_test_active = 0;
        mira_test_step = 0;
        mira_test_score = 0;
        return;
    }
}

/* -------------------------------------------------------
 * Mira Main Processor
 * ------------------------------------------------------- */
const char* mira_process(const char *msg)
{
    static char out[512];
    memset(out, 0, sizeof(out));

    if (!msg) {
        mira_say(out, sizeof(out),
                 "[Mira📐] Error: input nahi mila.",
                 "[Mira📐] Error: No input.");
        return out;
    }

    if (strcmp(msg, "help") == 0) {
        mira_help(out, sizeof(out));
        return out;
    }

    if (strcmp(msg, "start") == 0) {
        mira_say(out, sizeof(out),
                 "[Mira📐] Chalo start karte hain.\nType karo: mira \"lesson 1\"",
                 "[Mira📐] Let's start.\nType: mira \"lesson 1\"");
        return out;
    }

    if (strcmp(msg, "lesson 1") == 0) {
        mira_lesson_1(out, sizeof(out));
        return out;
    }

    if (strcmp(msg, "lesson 2") == 0) {
        mira_lesson_2(out, sizeof(out));
        return out;
    }

    if (strcmp(msg, "next") == 0) {
        mira_next(out, sizeof(out));
        return out;
    }

    if (strcmp(msg, "repeat") == 0) {
        mira_repeat_last(out, sizeof(out));
        return out;
    }

    if (strcmp(msg, "test 1") == 0 || mira_test_active) {
        mira_test_handler(out, sizeof(out), msg);
        return out;
    }

    int a, b;
    if (sscanf(msg, "%d+%d", &a, &b) == 2) {
        snprintf(out, sizeof(out),
                 "[Mira📐] %d + %d = %d",
                 a, b, a + b);
        return out;
    }

    if (strstr(msg, "solve")) {
        mira_say(out, sizeof(out),
                 "[Mira📐] Kya solve karu? Expression do.",
                 "[Mira📐] Solve what? Give a math expression.");
        return out;
    }

    mira_say(out, sizeof(out),
             "[Mira📐] Mujhe abhi koi math/learning pattern nahi dikh raha.\nTry: mira \"help\"",
             "[Mira📐] I see no math or learning pattern.\nTry: mira \"help\"");
    return out;
}
