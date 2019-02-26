// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSREADER_PHOTONSREADERWRAPPER_INTERNALPHOTONSOURCEWRAPPER_H_
#define PHOTONSREADER_PHOTONSREADERWRAPPER_INTERNALPHOTONSOURCEWRAPPER_H_

#include "BasicWrapper.h"
#include <vector>
#include <string>

namespace merlict {

class InternalPhotonSourceWrapper: public BasicWrapper {
    bool the_single_event_was_already_read;
    std::vector<Photon> photons;
 public:
    explicit InternalPhotonSourceWrapper(const std::string path);
    bool has_still_photons_left()const;
    std::vector<Photon> next(random::Generator* prng);
};

}  // namespace merlict

#endif  // PHOTONSREADER_PHOTONSREADERWRAPPER_INTERNALPHOTONSOURCEWRAPPER_H_
