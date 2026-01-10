#include "niyu.h"
#include <stdio.h>
#include <string.h>

// -------------------------------------------------------------
// GLOBAL last message for Consistency Engine
// -------------------------------------------------------------
const char *niyu_last_message_global = NULL;

// -------------------------------------------------------------
// INTERNAL MEMORY BUFFER (Circular Buffer v2.51)
// -------------------------------------------------------------
NiyuMemory niyu_memory = {
    .write_index = 0,
    .total = 0
};


// -------------------------------------------------------------
// STORE MESSAGE INTO MEMORY BUFFER
// -------------------------------------------------------------
void niyu_memory_store(const char *msg) {

    printf("[Memory📚] Saved: %s\n", msg);

    // Detect repeated messages
    for (int i = 0; i < niyu_memory.total; i++) {
        if (strcmp(niyu_memory.entries[i], msg) == 0) {
            printf("[Memory📚] Pattern: Repeated thought detected.\n");
            break;
        }
    }

    // Copy into buffer (safe)
    strncpy(niyu_memory.entries[niyu_memory.write_index], msg, 511);
    niyu_memory.entries[niyu_memory.write_index][511] = '\0';

    // Update global last message pointer
    niyu_last_message_global = niyu_memory.entries[niyu_memory.write_index];

    // Move circular index
    niyu_memory.write_index = (niyu_memory.write_index + 1) % NIYU_MEMORY_LIMIT;

    // Increase total count
    if (niyu_memory.total < NIYU_MEMORY_LIMIT)
        niyu_memory.total++;

    // Detect negative loops
    if (strstr(msg, "can't") || strstr(msg, "not possible") || strstr(msg, "stuck")) {
        printf("[Memory📚] Note: Negative loop found → Redirecting to logic.\n");
    }

    // Detect goals
    if (strstr(msg, "want") || strstr(msg, "goal") || strstr(msg, "wish")) {
        printf("[Memory📚] Tracking goals: Logged.\n");
    }
}



// -------------------------------------------------------------
// SAFE ACCESSOR → GET MEMORY ENTRY BY INDEX
// -------------------------------------------------------------
const char* niyu_memory_get(int index) {
    if (index < 0 || index >= niyu_memory.total)
        return NULL;
    return niyu_memory.entries[index];
}



// -------------------------------------------------------------
// RETURNS TOTAL STORED MESSAGES COUNT
// -------------------------------------------------------------
int niyu_memory_count() {
    return niyu_memory.total;
}



// -------------------------------------------------------------
// MEMORY DUMP (v2.51)
// -------------------------------------------------------------
void niyu_memory_dump() {

    printf("\n================ MEMORY DUMP (Niyu v2.51) ================\n");
    printf("Total Stored: %d / %d\n", niyu_memory.total, NIYU_MEMORY_LIMIT);

    for (int i = 0; i < niyu_memory.total; i++) {
        printf("%3d: %s\n", i + 1, niyu_memory.entries[i]);
    }

    printf("================ END OF MEMORY DUMP ======================\n\n");
}
