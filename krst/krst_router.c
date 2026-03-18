#include <stdio.h>
#include <string.h>

#include "../include/krst_router.h"
#include "../krst/krst_core.h"
#include "../include/pragya_avastha.h"

void krst_route_request(const KRSTRequest *req)
{
    if (!req || !req->input_text)
        return;

    /* 🔥 STEP 1: create avastha */
    PragyaAvastha avastha;
    memset(&avastha, 0, sizeof(PragyaAvastha));

    avastha.raw_text = req->input_text;
    avastha.has_correction = 0;

    /* 🔥 STEP 2: run KRST core */
    krst_process(req);

    /* 🔥 STEP 3: check correction */
    if (avastha.has_correction)
    {
        KRSTRequest new_req;
        new_req.input_text = avastha.corrected_text;

        printf("\n[KRST] Re-running corrected input...\n\n");

        krst_route_request(&new_req);
        return;
    }
}
