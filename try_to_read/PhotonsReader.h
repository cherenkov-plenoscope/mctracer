// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSREADER_PHOTONSREADER_H_
#define PHOTONSREADER_PHOTONSREADER_H_

#include <vector>
#include <string>
#include "PhotonsReaderWrapper/BasicWrapper.h"

// read in photons from any file
// All known formats of the mctracer will try to read in the file.
// Only the raw photon information is provided here.
// When reading e.g. Air shower cherenkov photons, and one wants to access the
// Monte Carlo truth aswell, you can not use this reader, but the specific
// reader, which is just wrapped here, itself.

namespace merlict {

class PhotonsReader {
    BasicWrapper *photons_from_file;
 public:
    explicit PhotonsReader(const std::string path);
    ~PhotonsReader();
    bool has_still_photons_left()const;
    std::vector<Photon> next(random::Generator* prng);
};

}  // namespace relleums

#endif  // PHOTONSREADER_PHOTONSREADER_H_
