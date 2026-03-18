#ifndef L3_RESPONSE_H
#define L3_RESPONSE_H

/*
 * L3Response:
 * Only Sakhi or Mira can create final response.
 * Niyu can never write directly here.
 */

typedef struct {
    const char *text;      /* final output */
    int         success;   /* logical success/failure */
} L3Response;

#endif /* L3_RESPONSE_H */
