// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORE_PROPAGATIONCONFIG_H_
#define MCTRACER_CORE_PROPAGATIONCONFIG_H_

#include "Color.h"
#include "Vec3.h"

struct PropagationConfig {
    unsigned int max_number_of_interactions_per_photon;
    bool use_multithread_when_possible;
    static const PropagationConfig default_config;
    PropagationConfig();
};
#endif  // MCTRACER_CORE_PROPAGATIONCONFIG_H_
