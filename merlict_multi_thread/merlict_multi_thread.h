// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_MULTI_THREAD_H_
#define MERLICT_MULTI_THREAD_H_

#include <vector>
#include <string>
#include "merlict/Photon.h"
#include "merlict/PropagationEnvironment.h"

namespace merlict {

void propagate_photons_in_frame_with_config_multi_thread(
    std::vector<Photon> *photons,
    std::shared_ptr<const Frame> world,
    const PropagationConfig* settings,
    random::Generator* prng);

}  // namespace merlict

#endif  // MERLICT_MULTI_THREAD_H_
