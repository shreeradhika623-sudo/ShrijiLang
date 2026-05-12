#include <stdio.h>
#include "../include/value.h"

/*
 * ShrijiLang Value Print System
 * --------------------------------
 * RULES:
 * - Final result MUST be printed with "OUTPUT:" prefix
 * - No empty printf("") calls
 * - Safe handling for NULL values
 * - Clean and extensible structure
 */

void value_print(Value v)
{
    // Prefix for UI parsing
    printf("OUTPUT: ");

    switch (v.type)
    {
        case VAL_STRING:
        {
            if (v.string != NULL)
                printf("%s", v.string);
            break;
        }

        case VAL_NUMBER:
        {
            printf("%g", v.number);
            break;
        }

        case VAL_BOOL:
        {
            printf("%s", v.boolean ? "true" : "false");
            break;
        }

        case VAL_LIST:
        {
            printf("[");
            for (int i = 0; i < v.count; i++)
            {
                if (i > 0)
                    printf(", ");

                Value item = v.items[i];

                if (item.type == VAL_NUMBER)
                {
                    printf("%g", item.number);
                }
                else if (item.type == VAL_STRING)
                {
                    if (item.string != NULL)
                        printf("\"%s\"", item.string);
                    else
                        printf("\"\"");
                }
                else if (item.type == VAL_BOOL)
                {
                    printf("%s", item.boolean ? "true" : "false");
                }
                else
                {
                    printf("null");
                }
            }
            printf("]");
            break;
        }

        default:
        {
            printf("null");
            break;
        }
    }

    // Always end with newline
    printf("\n");
}
