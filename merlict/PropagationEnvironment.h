// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_PROPAGATIONENVIRONMENT_H_
#define CORE_PROPAGATIONENVIRONMENT_H_

#include "merlict/Frame.h"
#include "merlict/PropagationConfig.h"
#include "merlict/random/random.h"

namespace merlict {

struct PropagationEnvironment {
    const Frame* root_frame = &VOID_FRAME;
    const PropagationConfig* config = &DEFAULT_PROPAGATIONCONFIG;
    random::Generator* prng = &random::VOID_PRNG;
};

}  // namespace relleums

#endif  // CORE_PROPAGATIONENVIRONMENT_H_
