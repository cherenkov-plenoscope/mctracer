// Copyright 2014 Sebastian A. Mueller
#include "Core/PropagationConfig.h"

const PropagationConfig PropagationConfig::default_config = PropagationConfig();

PropagationConfig::PropagationConfig() {
    use_multithread_when_possible = true;
    max_number_of_interactions_per_photon = 5;
}
