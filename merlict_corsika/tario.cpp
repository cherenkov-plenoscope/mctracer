// Copyright 2020 Sebastian A. Mueller
#include "merlict_corsika/tario.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include "merlict_corsika/corsika.h"

namespace tario {

Run::Run(std::string path) {
    this->_has_still_events_left = true;
    this->path = path;
    int rc = mtar_open(&this->tar, this->path.c_str(), "r");
    if (rc != MTAR_ESUCCESS) {
        std::stringstream out;
        out << "Failed to open/read TAR: '" << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    mtar_header_t runh_header;
    if (mtar_read_header(&this->tar, &runh_header) != MTAR_ESUCCESS) {
        std::stringstream out;
        out << "Failed to read tar-header of CORSIKA-run-header from TAR: '";
        out << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    std::string tar_header_name(runh_header.name);
    if (tar_header_name.compare("runh.float32") != 0) {
        std::stringstream out;
        out << "Expected first file to be 'runh.float32'. But it is '";
        out << tar_header_name << "', Path: '" << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    if (mtar_read_data(&tar, &this->runh, runh_header.size) != MTAR_ESUCCESS) {
        std::stringstream out;
        out << "Failed to read CORSIKA-run-header payload from TAR: '";
        out << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    if (runh[0] != corsika::str2float("RUNH")) {
        std::stringstream out;
        out << "Expected runh[0] == 'RUNH'. Path: ";
        out << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    _try_read_next_evth();
}

Run::~Run() {
    mtar_close(&this->tar);
}

void Run::_try_read_next_evth() {
    if (mtar_next(&this->tar) != MTAR_ESUCCESS) {
        std::stringstream out;
        out << "Failed to read next file (evth) from TAR: '";
        out << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    int rc = mtar_read_header(&this->tar, &this->next_tar_header_evth);
    if (rc == MTAR_ESUCCESS) {
        this->_has_still_events_left = true;
    } else {
        this->_has_still_events_left = false;
    }
}

bool Run::has_still_events_left() {
    return _has_still_events_left;
}

Event Run::next_event() {
    Event event;

    const std::string EVENT_HEADER_SUFFIX = ".evth.float32";
    const uint64_t PATH_NUM_DIGITS = 9;
    const uint64_t EVENT_HEADER_PATH_LENGTH =
        PATH_NUM_DIGITS + EVENT_HEADER_SUFFIX.length();
    std::string tar_header_evth_name(this->next_tar_header_evth.name);
    if (tar_header_evth_name.length() != EVENT_HEADER_PATH_LENGTH) {
        std::stringstream out;
        out << "Expected length of event-header-path to be ";
        out << EVENT_HEADER_PATH_LENGTH << ", but length of '";
        out << tar_header_evth_name << "' ";
        out << tar_header_evth_name.length();
        out << ", Path: '" << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    int suffix_match = tar_header_evth_name.compare(
        PATH_NUM_DIGITS,
        EVENT_HEADER_PATH_LENGTH,
        EVENT_HEADER_SUFFIX);
    if (suffix_match != 0) {
        std::stringstream out;
        out << "Expected evth path to have suffix : '";
        out << EVENT_HEADER_SUFFIX << "', but actual path is: '";
        out << tar_header_evth_name << ", Path: '" << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    if (mtar_read_data(
        &tar,
        &event.header,
        this->next_tar_header_evth.size) != MTAR_ESUCCESS
    ) {
        std::stringstream out;
        out << "Failed to read CORSIKA-event-header payload from TAR: '";
        out << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    if (event.header[0] != corsika::str2float("EVTH")) {
        std::stringstream out;
        out << "Expected evth[0] == 'EVTH'. Path: ";
        out << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    if (mtar_next(&this->tar) != MTAR_ESUCCESS) {
        std::stringstream out;
        out << "Failed to read next file (photon_bunches) from TAR: '";
        out << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    mtar_header_t bunch_header;
    if (mtar_read_header(&this->tar, &bunch_header) != MTAR_ESUCCESS) {
        std::stringstream out;
        out << "Failed to read tar-header of CORSIKA-bunches from TAR: '";
        out << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    const std::string BUNCH_SUFFIX = ".cherenkov_bunches.Nx8_float32";
    const uint64_t BUNCH_PATH_LENGTH = PATH_NUM_DIGITS + BUNCH_SUFFIX.length();
    std::string tar_header_bunch_name(bunch_header.name);
    if (tar_header_bunch_name.length() != BUNCH_PATH_LENGTH) {
        std::stringstream out;
        out << "Expected length of bunch-path to be ";
        out << BUNCH_PATH_LENGTH << ", but length of '";
        out << tar_header_bunch_name << "' ";
        out << tar_header_bunch_name.length();
        out << ", Path: '" << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    suffix_match = tar_header_bunch_name.compare(
        PATH_NUM_DIGITS,
        BUNCH_PATH_LENGTH,
        BUNCH_SUFFIX);
    if (suffix_match != 0) {
        std::stringstream out;
        out << "Expected bunch path to have suffix : '";
        out << BUNCH_SUFFIX << "', but actual path is: '";
        out << tar_header_bunch_name << ", Path: '" << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    int id_match = tar_header_bunch_name.compare(
        0,
        PATH_NUM_DIGITS,
        tar_header_evth_name.substr(0, PATH_NUM_DIGITS));
    if (id_match != 0) {
        std::stringstream out;
        out << "Expected evth path, and bunch path to have same id, but ";
        out << "bunch path: '" << tar_header_bunch_name << "', and ";
        out << "evth path: '" << tar_header_evth_name;
        out << "', Path: '" << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    const uint64_t SIZE_OF_BUNCH = 8*sizeof(float);
    const uint64_t size_of_bunches = bunch_header.size;
    const uint64_t size_rest = size_of_bunches%SIZE_OF_BUNCH;
    const uint64_t num_bunches = size_of_bunches/SIZE_OF_BUNCH;

    if (size_rest != 0) {
        std::stringstream out;
        out << "Expected size of bunches to be an exact multiple of ";
        out << "sizeof(bunch), but actual size of bunches :";
        out << size_of_bunches << ", and size of single bunch: ";
        out << SIZE_OF_BUNCH << ". This gives rest " << size_rest;
        out << "', Path: '" << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    event.photon_bunches.resize(num_bunches);
    if (mtar_read_data(
            &tar,
            event.photon_bunches.data(),
            bunch_header.size) != MTAR_ESUCCESS
    ) {
        std::stringstream out;
        out << "Failed to read CORSIKA-run-header payload from TAR: '";
        out << this->path << "'\n";
        throw std::runtime_error(out.str());
    }

    _try_read_next_evth();
    return event;
}


}  // namespace tario
