// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_PROPAGATIONENVIRONMENT_H_
#define CORE_PROPAGATIONENVIRONMENT_H_

#include "Core/Frame.h"
#include "Core/PropagationConfig.h"
#include "Core/Random/Random.h"

namespace relleums {

struct PropagationEnvironment {
    const Frame* root_frame = &VOID_FRAME;
    const PropagationConfig* config = &DEFAULT_PROPAGATIONCONFIG;
    random::Generator* prng = &random::VOID_PRNG;
};

}  // namespace relleums

#endif  // CORE_PROPAGATIONENVIRONMENT_H_
