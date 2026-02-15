#include <stdio.h>
#include "l3/router/l3_router.h"

int main(void)
{
    const char *input = "hello l3";

    L3Response res = l3_route_request(input);

    printf("L3 TEST OUTPUT:\n");
    printf("success = %d\n", res.success);
    printf("text    = %s\n", res.text ? res.text : "(null)");

    return 0;
}
