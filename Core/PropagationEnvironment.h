// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_PROPAGATIONENVIRONMENT_H_
#define CORE_PROPAGATIONENVIRONMENT_H_

#include "Core/Frame.h"
#include "Core/PropagationConfig.h"
#include "Core/Random/Random.h"

struct PropagationEnvironment {
    const Frame* root_frame = &Frame::VOID_FRAME;
    const PropagationConfig* config = &PropagationConfig::DEFAULT_CONFIG;
    Random::Generator* prng = &Random::VOID_PRNG;
    static PropagationEnvironment VOID_ENVIRONMENT;
};
#endif  // CORE_PROPAGATIONENVIRONMENT_H_
