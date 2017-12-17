// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_PROPAGATIONENVIRONMENT_H_
#define CORE_PROPAGATIONENVIRONMENT_H_

#include "Core/Frame.h"
#include "Core/PropagationConfig.h"
#include "Core/Random/Random.h"

struct PropagationEnvironment {
    const Frame* world_geometry = &Frame::VOID_FRAME;
    const PropagationConfig* propagation_options =
        &PropagationConfig::DEFAULT_CONFIG;
    Random::Generator* random_engine =
        &Random::void_generator;
    static PropagationEnvironment default_environment;
};
#endif  // CORE_PROPAGATIONENVIRONMENT_H_
