#include <stdio.h>
#include <string.h>

/*
    Build readable example from user input
*/

void shriji_build_example(const char *input, char *out, int size)
{
    int i = 0;
    int j = 0;

    while (input[i] && j < size - 1)
    {
        char c = input[i];

        if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            if (j < size - 3)
            {
                out[j++] = ' ';
                out[j++] = c;
                out[j++] = ' ';
            }
        }
        else
        {
            out[j++] = c;
        }

        i++;
    }

    out[j] = '\0';
}
