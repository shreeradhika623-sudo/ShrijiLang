// JAI SHREE KRISHNA
#include "mira_contract.h"
#include "../niyu/niyu_contract.h"

#include "../../include/error.h"
#include "../../include/pragya_avastha.h"

#include <stdio.h>
#include <string.h>


/* =========================================
   POINTER PRINTER
========================================= */

static void print_pointer(const char *text, int pos)
{
    if (!text || pos < 0)
        return;

    printf("%s\n", text);

    for (int i = 0; i < pos; i++)
        printf(" ");

    printf("^\n\n");
}


/* =========================================
   MIRA TEACHING ENGINE
========================================= */

void mira_teach_avastha(PragyaAvastha *a, NiyuResult *res)
{
    if (!a || !res)
        return;

    const char *text = a->raw_text;


    /* valid input → silent */

    if (res->error_type == NIYU_ERR_VALID)
        return;


    switch (a->error_code)
    {

    /* =========================================
       DOUBLE OPERATOR
    ========================================= */

    case E_PARSE_DOUBLE_OPERATOR:
    case E_PARSE_OPERATOR_CHAIN:

        printf("[SHRI_JI] Lagta hai yahaan extra operator aa gaya hai \n\n");

        print_pointer(text, res->error_pos);

        printf("Do operators ek saath allowed nahi hote.\n\n");

        printf("Example:\n");
        printf("6 + 5\n");

        break;


    /* =========================================
       OPERATOR AT START
    ========================================= */

    case E_PARSE_OPERATOR_START:

        printf("[SHRI_JI] Expression operator se start nahi hota \n\n");

        print_pointer(text, res->error_pos);

        printf("Pehle ek value honi chahiye.\n\n");

        printf("Example:\n");
        printf("6 + 5\n");

        break;


    /* =========================================
       OPERATOR AT END
    ========================================= */

    case E_PARSE_OPERATOR_END:

        printf("[SHRI_JI] Expression incomplete lag raha hai \n\n");

        print_pointer(text, res->error_pos);

        printf("Operator ke baad ek value expected hoti hai.\n\n");

        printf("Example:\n");
        printf("6 + 5\n");

        break;


    /* =========================================
       MISSING OPERATOR
    ========================================= */

    case E_PARSE_MISSING_OPERATOR:

        printf("[SHRI_JI] Lagta hai operator missing hai \n\n");

        print_pointer(text, res->error_pos);

        printf("Do values ke beech operator hona chahiye.\n\n");

        printf("Example:\n");
        printf("5 + 6\n");

        break;


    /* =========================================
       ASSIGNMENT ERROR
    ========================================= */

    case E_ASSIGN_02:

        printf("[SHRI_JI] Assignment incomplete lag raha hai.\n\n");

        print_pointer(text, res->error_pos);

        printf("'=' ke baad value expected hai.\n\n");

        printf("Example:\n");
        printf("mavi x = 10\n");

        break;


    /* =========================================
       GENERIC SYNTAX ERROR
    ========================================= */

    case E_PARSE_02:

        printf("[SHRI_JI] Syntax error detect hua hai.\n\n");

        print_pointer(text, res->error_pos);

        printf("Yahan invalid token ya unsupported symbol mila hai.\n\n");

        printf("Example valid syntax:\n");
        printf("6 + 5\n");

        break;


    /* =========================================
       DEFAULT
    ========================================= */

default:
    return;
    }
}
