// Copyright 2014 Sebastian A. Mueller
#include "EventIoWrapper.h"
#include "merlict_corsika/PhotonFactory.h"

namespace merlict {

merlict::EventIoWrapper::EventIoWrapper(const std::string _path):
    BasicWrapper(_path),
    corsika_run(_path) {}

bool EventIoWrapper::has_still_photons_left()const {
    return corsika_run.has_still_events_left();
}

std::vector<Photon> EventIoWrapper::next(random::Generator* prng) {
    eventio::Event event = corsika_run.next_event();
    std::vector<Photon> photons;
    unsigned int photon_id = 0;
    for (std::array<float, 8> corsika_photon : event.photons) {
        EventIoPhotonFactory cpf(corsika_photon, photon_id++, prng);
        if (cpf.passed_atmosphere())
            photons.push_back(cpf.make_photon());
    }
    return photons;
}

}  // namespace merlict
