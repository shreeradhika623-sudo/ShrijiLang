#include <stdio.h>
#include "../include/value.h"

void value_print(Value v)
{
    if (v.type == VAL_STRING) {
        printf("%s\n", v.string ? v.string : "");
    }
    else if (v.type == VAL_NUMBER) {
        printf("%g\n", v.number);
    }
    else if (v.type == VAL_BOOL) {
        printf("%d\n", v.boolean);
    }
    else if (v.type == VAL_LIST) {
        printf("[");
        for (int i = 0; i < v.count; i++) {
            if (i > 0) printf(", ");

            if (v.items[i].type == VAL_NUMBER)
                printf("%g", v.items[i].number);
            else if (v.items[i].type == VAL_STRING)
                printf("\"%s\"", v.items[i].string);
            else if (v.items[i].type == VAL_BOOL)
                printf("%d", v.items[i].boolean);
            else
                printf("?");
        }
        printf("]\n");
    }
    else {
        printf("0\n");
    }
}
