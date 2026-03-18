#include <stdio.h>
#include <string.h>

#include "../include/fix_interactive.h"
#include "../include/error.h"

/* =========================================================
   SAFE INPUT
========================================================= */

static int read_line(char *buffer, int size)
{
    if (!fgets(buffer, size, stdin))
        return 0;

    buffer[strcspn(buffer, "\n")] = '\0';
    return 1;
}

/* =========================================================
   DOUBLE OPERATOR FIX
========================================================= */

int shriji_interactive_double_operator_fix(
    const char *input,
    char *output,
    int size)
{
    char left[128];
    char op1[8];
    char op2[8];
    char right[128];

    if (sscanf(input,"%127[^+-*/]%7[+-*/]%7[+-*/]%127s",
               left,op1,op2,right)!=4)
        return 0;

    printf("\nHmm... yahan thoda sa mix-up ho gaya hai 🙂\n\n");
    printf("Shayad aap kehna chahte the:\n\n");

    printf("A) %s %s %s\n",left,op1,right);
    printf("B) %s %s %s\n",left,op2,right);

    printf("L) '%s' hata du\n",op1);
    printf("R) '%s' hata du\n",op2);

    printf("\nI) Ignore\n");
    printf("M) Manual fix\n");

    while (1)
    {
        printf("\nSelect option: ");

        char choice=getchar();
        getchar();

        if(choice=='A'||choice=='a')
        {
            snprintf(output,size,"%s %s %s",left,op1,right);
            return 1;
        }

        if(choice=='B'||choice=='b')
        {
            snprintf(output,size,"%s %s %s",left,op2,right);
            return 1;
        }

        if(choice=='L'||choice=='l')
        {
            snprintf(output,size,"%s %s %s",left,op2,right);
            return 1;
        }

        if(choice=='R'||choice=='r')
        {
            snprintf(output,size,"%s %s %s",left,op1,right);
            return 1;
        }

        if(choice=='M'||choice=='m')
        {
            printf("Type corrected expression: ");

            if(!read_line(output,size))
                return 0;

            return 1;
        }

        if(choice=='I'||choice=='i')
            return 0;

        printf("\nHmm... shayad galti se wrong option press ho gaya 🙂\n");
        printf("Tension mat lo, dobara ek option choose karo.\n");
    }
}

/* =========================================================
   INTERACTIVE BRACKET FIX
========================================================= */

int shriji_interactive_bracket_fix(
    const char *input,
    char *output,
    int size)
{
    while (1)
    {
        printf("\nPossible fixes:\n\n");

        printf("A) Add missing ')'\n");
        printf("R) Remove extra '('\n");

        printf("\nI) Ignore\n");
        printf("M) Manual fix\n");

        printf("\nSelect option: ");

        char choice=getchar();
        getchar();

        if(choice=='A'||choice=='a')
        {
            snprintf(output,size,"%s)",input);
            return 1;
        }

        if(choice=='R'||choice=='r')
        {
            int j=0;

            for(int i=0;input[i];i++)
            {
                if(input[i]=='(')
                    continue;

                output[j++]=input[i];
            }

            output[j]='\0';
            return 1;
        }

        if(choice=='M'||choice=='m')
        {
            printf("Type corrected expression: ");

            if(!read_line(output,size))
                return 0;

            return 1;
        }

        if(choice=='I'||choice=='i')
            return 0;

        printf("\nHmm... shayad galti se wrong option press ho gaya 🙂\n");
        printf("Tension mat lo, dobara ek option choose karo.\n");
    }
}

/* =========================================================
   INTERACTIVE OPERATOR FIX
========================================================= */

int shriji_interactive_operator_fix(
    const char *input,
    char *output,
    int size)
{
    char left[128];
    char right[128];

    if (sscanf(input, "%127s %127s", left, right) != 2)
        return 0;

    printf("\nPossible fixes:\n\n");

    printf("A) %s + %s\n", left, right);
    printf("B) %s - %s\n", left, right);
    printf("C) %s * %s\n", left, right);
    printf("D) %s / %s\n", left, right);

    printf("\nI) Ignore\n");
    printf("M) Manual fix\n");

    while (1)
    {
        printf("\nSelect option: ");

        char choice=getchar();
        getchar();

        const char *op=NULL;

        if(choice=='A'||choice=='a') op="+";
        else if(choice=='B'||choice=='b') op="-";
        else if(choice=='C'||choice=='c') op="*";
        else if(choice=='D'||choice=='d') op="/";
        else if(choice=='I'||choice=='i') return 0;

        if(choice=='M'||choice=='m')
        {
            printf("Type corrected expression: ");

            if(!read_line(output,size))
                return 0;

            return 1;
        }

        if(op)
        {
            snprintf(output,size,"%s %s %s",left,op,right);
            return 1;
        }

        printf("\nHmm... shayad galti se wrong option press ho gaya 🙂\n");
        printf("Tension mat lo, dobara option choose karo.\n");
    }
}
