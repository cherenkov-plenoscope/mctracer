// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_PROPAGATIONCONFIG_H_
#define CORE_PROPAGATIONCONFIG_H_

#include "Color.h"
#include "Vec3.h"

struct PropagationConfig {
    unsigned int max_number_of_interactions_per_photon;
    bool use_multithread_when_possible;
    static const PropagationConfig DEFAULT_CONFIG;
    PropagationConfig();
};
#endif  // CORE_PROPAGATIONCONFIG_H_
