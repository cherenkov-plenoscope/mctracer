// Copyright 2020 Sebastian A. Mueller
#ifndef MERLICT_DEVELOPMENT_KIT_MERLICT_CORSIKA_EVENT_TAPE_H_
#define MERLICT_DEVELOPMENT_KIT_MERLICT_CORSIKA_EVENT_TAPE_H_

#include <assert.h>
#include <string>
#include <vector>
#include <array>
#include <fstream>

extern "C" {
#include "merlict_corsika/mli_corsika_EventTape_standalone.h"
}

namespace event_tape {

struct Event {
    std::array<float, 273> header;
    std::vector<std::array<float, 8>> photons;
};

struct Run {
    std::string path;
    struct mliEventTapeReader etr;
    std::array<float, 273> header;

    std::array<float, 273> next_evth;
    bool _has_still_events_left;

    explicit Run(std::string path);
    ~Run();

    Event next_event();
    bool has_still_events_left();
    void _try_read_next_evth();
};

}  // namespace event_tape

#endif  // MERLICT_DEVELOPMENT_KIT_MERLICT_CORSIKA_EVENT_TAPE_H_
