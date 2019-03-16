// Copyright 2014 Sebastian A. Mueller
#include "merlict/Photons.h"
#include <sstream>
#include <future>
#include <thread>
#include "merlict_multi_thread/vitaliy_vitsentiy_thread_pool.h"
#include "merlict/PhotonAndFrame.h"


namespace merlict {

void __propagate_one_photon(
    int id,
    const Frame* world,
    const PropagationConfig* settings,
    const uint64_t seed,
    Photon* photon
) {
    (void)id;
    random::Mt19937 prng(seed);
    PropagationEnvironment env;
    env.root_frame = world;
    env.config = settings;
    env.prng = &prng;
    Propagator(photon, env);
}

void propagate_photons_in_frame_with_config_multi_thread(
    std::vector<Photon> *photons,
    const Frame* world,
    const PropagationConfig* settings,
    random::Generator* prng
) {
    std::vector<uint64_t> prng_seeds(photons->size());
    for (uint64_t i = 0; i < photons->size(); i ++) {
        prng_seeds[i] = prng->create_seed();
    }

    uint64_t num_threads = std::thread::hardware_concurrency();
    ctpl::thread_pool pool(num_threads);
    std::vector<std::future<void>> results(photons->size());

    for (uint64_t i = 0; i < photons->size(); ++i) {
        results[i] = pool.push(
            __propagate_one_photon,
            world,
            settings,
            prng_seeds[i],
            &(*photons)[i]);
    }

    for (uint64_t i = 0; i < photons->size(); i ++) {
        results[i].get();
    }
}

}  // namespace merlict
