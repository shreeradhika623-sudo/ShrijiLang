#include "../include/smriti_personal.h"
#include <string.h>
#include <stdio.h>

#define MAX_KEYS 100

typedef struct {
    char key[32];
    char value[128];
} MemoryItem;

/* ===============================
   INTERNAL MEMORY (RAM)
   =============================== */
static MemoryItem memory[MAX_KEYS];
static int memory_count = 0;

/* ===============================
   FILE PATH
   =============================== */
static const char *SMRITI_FILE = ".shriji/smriti.db";

/* ===============================
   LOAD FROM FILE
   =============================== */
void smriti_personal_load(void)
{
    FILE *fp = fopen(SMRITI_FILE, "r");
    if (!fp) return;

    char line[256];

    while (fgets(line, sizeof(line), fp))
    {
        char *eq = strchr(line, '=');
        if (!eq) continue;

        *eq = '\0';

        char *key = line;
        char *value = eq + 1;

        /* remove newline */
        value[strcspn(value, "\n")] = 0;

        smriti_personal_set(key, value);
    }

    fclose(fp);
}

/* ===============================
   SAVE TO FILE
   =============================== */
void smriti_personal_save(void)
{
    FILE *fp = fopen(SMRITI_FILE, "w");
    if (!fp) return;

    for (int i = 0; i < memory_count; i++)
    {
        fprintf(fp, "%s=%s\n", memory[i].key, memory[i].value);
    }

    fclose(fp);
}

/* ===============================
   SET (WRITE)
   =============================== */
void smriti_personal_set(const char *key, const char *value)
{
    if (!key || !value) return;

    /* overwrite if exists */
    for (int i = 0; i < memory_count; i++)
    {
        if (strcmp(memory[i].key, key) == 0)
        {
            strncpy(memory[i].value, value, sizeof(memory[i].value)-1);
            memory[i].value[sizeof(memory[i].value)-1] = '\0';

            smriti_personal_save();
            return;
        }
    }

    /* new entry */
    if (memory_count < MAX_KEYS)
    {
        strncpy(memory[memory_count].key, key, sizeof(memory[memory_count].key)-1);
        strncpy(memory[memory_count].value, value, sizeof(memory[memory_count].value)-1);

        memory[memory_count].key[sizeof(memory[memory_count].key)-1] = '\0';
        memory[memory_count].value[sizeof(memory[memory_count].value)-1] = '\0';

        memory_count++;

        smriti_personal_save();
    }
}

/* ===============================
   GET (READ)
   =============================== */
const char* smriti_personal_get(const char *key)
{
    if (!key) return NULL;

    for (int i = 0; i < memory_count; i++)
    {
        if (strcmp(memory[i].key, key) == 0)
        {
            return memory[i].value;
        }
    }

    return NULL;
}

/* ===============================
   CLEAR (RAM ONLY)
   =============================== */
void smriti_personal_clear(void)
{
    memory_count = 0;
}
