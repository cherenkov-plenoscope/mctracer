// Copyright 2014 Sebastian A. Mueller
#include "merlict/PropagationConfig.h"

namespace merlict {

PropagationConfig::PropagationConfig() {
    use_multithread_when_possible = true;
    max_number_of_interactions_per_photon = 5;
}

}  // namespace merlict
