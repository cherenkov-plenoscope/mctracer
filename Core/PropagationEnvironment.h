// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORE_PROPAGATIONENVIRONMENT_H_
#define MCTRACER_CORE_PROPAGATIONENVIRONMENT_H_

#include "Core/Frame.h"
#include "Core/PropagationConfig.h"
#include "Core/Random/Random.h"

class PropagationEnvironment {
 public:
    const Frame* world_geometry = &Frame::VOID_FRAME;
    const PropagationConfig* propagation_options =
        &PropagationConfig::DEFAULT_CONFIG;
    Random::Generator* random_engine =
        &Random::void_generator;
    static PropagationEnvironment default_environment;
};
#endif  // MCTRACER_CORE_PROPAGATIONENVIRONMENT_H_
