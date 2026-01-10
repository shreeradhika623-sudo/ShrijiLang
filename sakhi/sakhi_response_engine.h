#ifndef SAKHI_RESPONSE_ENGINE_H
#define SAKHI_RESPONSE_ENGINE_H

#include "sakhi.h"
#include "sakhi_states.h"
#include "sakhi_context.h"
#include "sakhi_intensity.h"

// Master emotional response generator
const char* sakhi_response_engine(
    SakhiEmotion emo,
    SakhiContext ctx,
    int intensity
);

#endif
