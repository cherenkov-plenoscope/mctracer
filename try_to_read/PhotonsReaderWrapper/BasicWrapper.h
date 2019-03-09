// Copyright 2014 Sebastian A. Mueller
#ifndef TRY_TO_READ_PHOTONSREADERWRAPPER_BASICWRAPPER_H_
#define TRY_TO_READ_PHOTONSREADERWRAPPER_BASICWRAPPER_H_

#include <vector>
#include <string>
#include "merlict/Photons.h"
#include "merlict/random/random.h"

namespace merlict {

class BasicWrapper {
 protected:
    const std::string path;
 public:
    explicit BasicWrapper(const std::string path);
    virtual ~BasicWrapper();
    virtual bool has_still_photons_left()const = 0;
    virtual std::vector<Photon> next(random::Generator* prng) = 0;
};

}  // namespace merlict

#endif  // TRY_TO_READ_PHOTONSREADERWRAPPER_BASICWRAPPER_H_
