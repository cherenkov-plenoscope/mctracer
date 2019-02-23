// Copyright 2014 Sebastian A. Mueller
#include <fstream>
#include <sstream>
#include "Corsika/EventIo/Export.h"
using std::string;
using std::vector;
using std::array;

namespace relleums {

void write_corsika_photons(
    const vector<array<float, 8>> &photons,
    const string &path
) {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "EventIo: Unable to write file: '" << path << "'\n";
        throw std::runtime_error(info.str());
    }

    for (const array<float, 8> &photon : photons)
        file.write((char*)&photon, sizeof(array<float, 8>));
    file.close();
}

}  // namespace relleums
