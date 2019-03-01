// Copyright 2014 Sebastian A. Mueller
#include "try_to_read/PhotonsReader.h"
#include <sstream>
#include "PhotonsReaderWrapper/EventIoWrapper.h"
#include "PhotonsReaderWrapper/AsciiIoWrapper.h"
#include "PhotonsReaderWrapper/InternalPhotonSourceWrapper.h"


namespace merlict {

PhotonsReader::PhotonsReader(const std::string path) {
    std::stringstream out;

    try {
        out << "___EventIo___\n";
        photons_from_file = new EventIoWrapper(path);
    } catch (std::exception &error) {
        out << error.what();
        try {
            out << "___AsciiIo___\n";
            photons_from_file = new AsciiIoWrapper(path);
        } catch (std::exception &error) {
            out << error.what();
            try {
                out << "___InternalLightSource___\n";
                photons_from_file = new InternalPhotonSourceWrapper(path);
            } catch (std::exception &error) {
                out << error.what();
                out <<
                "Can not read photons from file, it is neither "
                "Corsika EventIo, nor Ascci, nor an internal source\n";
                throw std::invalid_argument(out.str());
            }
        }
    }
}

PhotonsReader::~PhotonsReader() {
    delete photons_from_file;
}

bool PhotonsReader::has_still_photons_left()const {
    return photons_from_file->has_still_photons_left();
}

std::vector<Photon> PhotonsReader::next(random::Generator* prng) {
    return photons_from_file->next(prng);
}

}  // namespace merlict
