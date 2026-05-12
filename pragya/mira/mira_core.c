// JAI SHREE KRISHNA
#include "mira_contract.h"
#include "../niyu/niyu_contract.h"

#include "../../include/error.h"
#include "../../include/pragya_avastha.h"

#include <stdio.h>
#include <string.h>

/* =========================================
   EXPLAIN PRINT (CORE)
========================================= */
static void explain(const char *text)
{
    if (!text) return;
    printf("EXPLAIN: %s", text);
}

/* =========================================
   POINTER PRINTER
========================================= */
static void print_pointer(const char *text, int pos)
{
    if (!text || pos < 0)
        return;

    printf("EXPLAIN: %s\n", text);

    printf("EXPLAIN: ");
    for (int i = 0; i < pos; i++)
        printf(" ");

    printf("^\n");
    printf("\n");
}

/* =========================================
   MIRA TEACHING ENGINE
========================================= */
void mira_teach_avastha(PragyaAvastha *a, NiyuResult *res)
{
    if (!a || !res)
        return;

    const char *text = a->raw_text;

    if (res->error_type == NIYU_ERR_VALID)
        return;

    switch (a->error_code)
    {

    case E_PARSE_DOUBLE_OPERATOR:
    case E_PARSE_OPERATOR_CHAIN:

        explain("Extra operator detect hua hai\n\n");

        print_pointer(text, res->error_pos);

        explain("Do operators ek saath allowed nahi hote.\n\n");
        explain("Example: 6 + 5\n");

        break;

    case E_PARSE_OPERATOR_START:

        explain("Expression operator se start nahi hota\n\n");

        print_pointer(text, res->error_pos);

        explain("Pehle ek value honi chahiye.\n\n");
        explain("Example: 6 + 5\n");

        break;

    case E_PARSE_OPERATOR_END:

        explain("Expression incomplete lag raha hai\n\n");

        print_pointer(text, res->error_pos);

        explain("Operator ke baad ek value expected hoti hai.\n\n");
        explain("Example: 6 + 5\n");

        break;

    case E_PARSE_MISSING_OPERATOR:

        explain("Operator missing hai\n\n");

        print_pointer(text, res->error_pos);

        explain("Do values ke beech operator hona chahiye.\n\n");
        explain("Example: 5 + 6\n");

        break;

    case E_ASSIGN_02:

        explain("Assignment incomplete hai\n\n");

        print_pointer(text, res->error_pos);

        explain("'=' ke baad value expected hai.\n\n");
        explain("Example: mavi x = 10\n");

        break;

    case E_PARSE_02:

        explain("Syntax error detect hua hai\n\n");

        print_pointer(text, res->error_pos);

        explain("Invalid token ya unsupported symbol mila hai.\n\n");
        explain("Example: 6 + 5\n");

        break;

    default:
        return;
    }
}
