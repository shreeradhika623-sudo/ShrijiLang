#ifndef LOG_MODE_H
#define LOG_MODE_H

#include <stdio.h>
#include "user_config.h"

/* ---------------------------------------------------------
 * LOGGING MACROS
 * DEV_MODE=1  => verbose prints
 * DEV_MODE=0  => silent
 * --------------------------------------------------------- */

#define NIYU_LOG(...)  do { if (DEV_MODE) printf(__VA_ARGS__); } while (0)
#define SAKHI_LOG(...) do { if (DEV_MODE) printf(__VA_ARGS__); } while (0)
#define MIRA_LOG(...)  do { if (DEV_MODE) printf(__VA_ARGS__); } while (0)
#define KAVYA_LOG(...) do { if (DEV_MODE) printf(__VA_ARGS__); } while (0)

#endif
