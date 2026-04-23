#include <stdio.h>
#include <string.h>
#include "../include/nirman.h"

void nirman_flow_process(char *line)
{
    int intent = nirman_detect_intent(line);

    /* STAGE 0 → START */
    if (intent == 2 && NIRMAN_CTX.flow_stage == 0)
    {
        printf("Aap kya banana chahte ho?\n");
        NIRMAN_CTX.flow_stage = 1;
        return;
    }

    /* STAGE 1 → GOAL CAPTURE */
    if (NIRMAN_CTX.flow_stage == 1)
    {
        if (strstr(line, "software") || strstr(line, "create"))
        {
            printf("Aap kya banana chahte ho?\n");
            return;
        }

        strncpy(NIRMAN_CTX.goal, line, sizeof(NIRMAN_CTX.goal) - 1);
        printf("[FLOW] Target set: %s\n", NIRMAN_CTX.goal);

        NIRMAN_CTX.flow_stage = 2;
        return;
    }

    /* STAGE 2 → ASK TYPE */
    if (NIRMAN_CTX.flow_stage == 2)
    {
        if (strstr(NIRMAN_CTX.goal, "calculator"))
        {
            printf("Aapko kis type ka calculator chahiye?\n");
            NIRMAN_CTX.flow_stage = 3;
            return;
        }
    }

    /* STAGE 3 → HANDLE TYPE */
    if (NIRMAN_CTX.flow_stage == 3)
    {
        if (strstr(NIRMAN_CTX.goal, "calculator"))
        {
            if (strstr(line, "normal"))
            {
                printf("[FLOW] Type selected: normal calculator\n");
                printf("Agla step: features define karte hain\n");

                NIRMAN_CTX.flow_stage = 4;
            }
            else if (strstr(line, "scientific"))
            {
                printf("[FLOW] Type selected: scientific calculator\n");
                printf("Agla step: features define karte hain\n");

                NIRMAN_CTX.flow_stage = 4;
            }
            else
            {
                printf("Type clear nahi hai (normal / scientific)\n");
            }
            return;
        }
    }

    /* STAGE 4 → NEXT STEP (placeholder) */
    if (NIRMAN_CTX.flow_stage == 4)
    {
        printf("[FLOW] Feature planning phase...\n");
        return;
    }
}
