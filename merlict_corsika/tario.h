// Copyright 2020 Sebastian A. Mueller
#ifndef MERLICT_DEVELOPMENT_KIT_MERLICT_CORSIKA_TARIO_H_
#define MERLICT_DEVELOPMENT_KIT_MERLICT_CORSIKA_TARIO_H_

#include <string>
#include <vector>
#include <array>
#include <fstream>
#include "merlict_corsika/microtar.h"

namespace tario {

struct Event {
    std::array<float, 273> header;
    std::vector<std::array<float, 8>> photons;
};

struct Run {
    std::string path;
    mtar_t tar;
    std::array<float, 273> header;

    mtar_header_t next_tar_header_evth;
    bool _has_still_events_left;

    explicit Run(std::string path);
    ~Run();

    Event next_event();
    bool has_still_events_left();
    void _try_read_next_evth();
};

}  // namespace tario

#endif  // MERLICT_DEVELOPMENT_KIT_MERLICT_CORSIKA_TARIO_H_
