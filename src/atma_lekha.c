#include "../include/atma_lekha.h"
#include <stdio.h>
#include <time.h>

static const char *ATMA_FILE = ".shriji/atma_lekha.log";

void atma_lekha_init(void)
{
    FILE *fp = fopen(ATMA_FILE, "a");
    if (fp) fclose(fp);
}

void atma_lekha_note(AtmaEventType type,
                     const char *component,
                     const char *detail)
{
    FILE *fp = fopen(ATMA_FILE, "a");
    if (!fp) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(fp,
        "[%04d-%02d-%02d %02d:%02d:%02d] | %d | %s | %s\n",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        type,
        component ? component : "unknown",
        detail ? detail : "-"
    );

    fclose(fp);
}
