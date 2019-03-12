// Copyright 2014 Sebastian A. Mueller
#ifndef TRY_TO_READ_PHOTONSREADERWRAPPER_EVENTIOWRAPPER_H_
#define TRY_TO_READ_PHOTONSREADERWRAPPER_EVENTIOWRAPPER_H_

#include <vector>
#include <string>
#include "BasicWrapper.h"
#include "merlict_corsika/eventio.h"

namespace merlict {

class EventIoWrapper: public BasicWrapper {
    eventio::Run corsika_run;
 public:
    explicit EventIoWrapper(const std::string path);
    bool has_still_photons_left()const;
    std::vector<Photon> next(random::Generator* prng);
};

}  // namespace merlict

#endif  // TRY_TO_READ_PHOTONSREADERWRAPPER_EVENTIOWRAPPER_H_
