// Copyright 2020 Sebastian A. Mueller
#include "merlict_corsika/event_tape.h"

extern "C" {
#include "merlict_corsika/mli_corsika_EventTape_standalone.c"
}

namespace event_tape {

Run::Run(std::string path) {
    this->path = path;
    this->etr = mliEventTapeReader_init();
    chk_msg(mliEventTapeReader_open(&this->etr, this->path.c_str()),
        "Can't open event-tape for reading.");
    chk_msg(mliEventTapeReader_read_runh(&this->etr, this->header.data()),
        "Can't read RUNH from event-tape.");
    this->_try_read_next_evth();
    return;
error:
    assert(false);
}

Run::~Run() {
    mliEventTapeReader_close(&this->etr);
}

void Run::_try_read_next_evth() {
    int rc = mliEventTapeReader_read_evth(&this->etr, this->next_evth.data());
    this->_has_still_events_left = rc == 1 ? true : false;
}

bool Run::has_still_events_left() {
    return this->_has_still_events_left;
}

Event Run::next_event() {
    Event event;
    event.header = this->next_evth;
    std::array<float, 8> bunch;
    while (mliEventTapeReader_read_cherenkov_bunch(&this->etr, bunch.data())) {
        event.photons.push_back(bunch);
    }
    this->_try_read_next_evth();
    return event;
}

}  // namespace event_tape
