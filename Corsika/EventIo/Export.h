// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORSIKA_EVENTIO_EXPORT_H_
#define MCTRACER_CORSIKA_EVENTIO_EXPORT_H_

#include <vector>
#include <array>
#include <string>

namespace relleums {

void write_corsika_photons(
    const std::vector<std::array<float, 8>> &photons,
    const std::string &path
);

}  // namespace relleums

#endif  // MCTRACER_CORSIKA_EVENTIO_EXPORT_H_
