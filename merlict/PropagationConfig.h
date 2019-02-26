// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_PROPAGATIONCONFIG_H_
#define CORE_PROPAGATIONCONFIG_H_

#include "Color.h"
#include "Vec3.h"

namespace merlict {

struct PropagationConfig {
    unsigned int max_number_of_interactions_per_photon;
    bool use_multithread_when_possible;
    PropagationConfig();
};

const PropagationConfig DEFAULT_PROPAGATIONCONFIG;

}  // namespace relleums

#endif  // CORE_PROPAGATIONCONFIG_H_
