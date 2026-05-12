#include <stdio.h>

#include "../include/krst_print.h"

/* =========================================
   KRST RESPONSE PRINTER
========================================= */

void krst_print_response(KRSTResponse *response)
{
    if (!response)
        return;

   switch (response->type)
{
    /* ================= NORMAL ================= */

  case KRST_RESPONSE_NORMAL:

    if (response->result_text[0] != '\0')
    {
        printf("OUTPUT:\n");
        printf("%s\n", response->result_text);
    }

    if (response->explain_text[0] != '\0')
    {
        printf("EXPLAIN:\n");
        printf("%s\n", response->explain_text);
    }

           break;

    /* ================= ERROR ================= */

    case KRST_RESPONSE_ERROR:

        printf("ERROR:\n");

        if (response->message[0] != '\0')
        {
            printf("%s\n\n", response->message);
        }

        if (response->details[0] != '\0')
        {
            printf("DETAILS:\n");
            printf("%s\n\n", response->details);
        }

        if (response->input[0] != '\0')
        {
            printf("INPUT:\n");
            printf("%s\n", response->input);

            if (response->pointer_col > 0)
            {
                for (int i = 1; i < response->pointer_col; i++)
                    printf(" ");

                printf("^\n");
            }

            printf("\n");
        }

    if (response->hint[0] != '\0')
{
    printf("SUGGESTION:\n");
    printf("%s\n", response->hint);
}

/* ================= JSON ERROR ================= */

 printf("\nJSON:\n");

  printf(
         "{\"success\":false,"
         "\"output\":\"\","
         "\"explain\":\"\","
         "\"error\":\"%s\"}\n",
         response->message
  );

      break;

    /* ================= WARNING ================= */

    case KRST_RESPONSE_WARNING:

        printf("WARNING:\n");

        if (response->message[0] != '\0')
        {
            printf("%s\n\n", response->message);
        }

        if (response->details[0] != '\0')
        {
            printf("DETAILS:\n");
            printf("%s\n\n", response->details);
        }

      printf("\nJSON:\n");

printf(
    "{\"success\":false,"
    "\"output\":\"\","
    "\"explain\":\"%s\","
    "\"error\":\"%s\"}\n",
    response->details,
    response->message
);


       break;

    /* ================= CRITICAL ================= */

    case KRST_RESPONSE_CRITICAL:

        printf("CRITICAL:\n");

        if (response->message[0] != '\0')
        {
            printf("%s\n\n", response->message);
        }

        if (response->details[0] != '\0')
        {
            printf("DETAILS:\n");
            printf("%s\n", response->details);
        }

     printf("\nJSON:\n");

printf(
    "{\"success\":false,"
    "\"output\":\"\","
    "\"explain\":\"%s\","
    "\"error\":\"%s\"}\n",
    response->details,
    response->message
); 
       break;


case KRST_RESPONSE_AUTO_FIX:

if (
    response->original_input[0] == '\0' &&
    response->corrected_input[0] == '\0'
)
{
    break;
}
    printf("AUTO FIX:\n");

    if (response->original_input[0] != '\0')
    {
        printf("INPUT:\n");
        printf("%s\n\n", response->original_input);
    }

    if (response->corrected_input[0] != '\0')
    {
        printf("CORRECTED:\n");
        printf("%s\n\n", response->corrected_input);
    }

    if (response->result_text[0] != '\0')
    {
        printf("OUTPUT:\n");
        printf("%s\n", response->result_text);
    }

      printf("\nJSON:\n");

printf(
    "{\"success\":true,"
    "\"output\":\"%s\","
    "\"explain\":\"Auto corrected expression\","
    "\"error\":\"\","
    "\"corrected\":\"%s\"}\n",
    response->result_text,
    response->corrected_input
);

    break;


    default:
        break;
}

}
