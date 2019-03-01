// Copyright 2014 Sebastian A. Mueller
#include "InternalPhotonSourceWrapper.h"
#include "json.h"


namespace merlict {

InternalPhotonSourceWrapper::InternalPhotonSourceWrapper(
	const std::string _path
):
    BasicWrapper(_path) {
    the_single_event_was_already_read = false;
    photons = json::load_photons(path);
}

bool InternalPhotonSourceWrapper::has_still_photons_left()const {
    return the_single_event_was_already_read? false: true;
}

std::vector<Photon> InternalPhotonSourceWrapper::next(random::Generator* prng) {
    (void)*prng;
    the_single_event_was_already_read = true;
    return photons;
}

}  // namespace merlict
