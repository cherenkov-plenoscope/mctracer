// Copyright 2014 Sebastian A. Mueller, Dominik Neise
#include <algorithm>
#include <sstream>
#include "merlict/tests/catch.hpp"
#include "merlict_corsika/event_tape.h"
#include "merlict_corsika/corsika.h"
#include "merlict_corsika/PhotonFactory.h"


TEST_CASE("EventTapeTest: read one by one", "[merlict]") {
    event_tape::Run run("merlict_corsika/tests/resources/run_event_tape.tar");
    CHECK(run.header[0] == corsika::str2float("RUNH"));
    CHECK(run.has_still_events_left());

    event_tape::Event first_evt = run.next_event();
    CHECK(first_evt.header[0] == corsika::str2float("EVTH"));
    CHECK(run.has_still_events_left());

    event_tape::Event second_evt = run.next_event();
    CHECK(second_evt.header[0] == corsika::str2float("EVTH"));
    CHECK(run.has_still_events_left());

    event_tape::Event third_evt = run.next_event();
    CHECK(third_evt.header[0] == corsika::str2float("EVTH"));
    CHECK(!run.has_still_events_left());
}

TEST_CASE("EventTapeTest: while loop", "[merlict]") {
    merlict::random::Mt19937 prng(0);
    event_tape::Run run("merlict_corsika/tests/resources/run_event_tape.tar");
    CHECK(run.header[0] == corsika::str2float("RUNH"));
    CHECK(run.has_still_events_left());

    while (run.has_still_events_left()) {
        event_tape::Event evt = run.next_event();
        CHECK(evt.header[0] == corsika::str2float("EVTH"));

        std::vector<merlict::Photon> photons;
        for (uint64_t i = 0; i < evt.photons.size(); i++) {
            merlict::EventIoPhotonFactory cpf(
                evt.photons[i],
                i,
                &prng);
            if (cpf.has_still_photons_to_be_made()) {
                photons.push_back(cpf.make_photon());
            }
        }

        for (uint64_t i = 0; i < photons.size(); i++) {
            CHECK(photons.at(i).wavelength > 190e-9);
            CHECK(photons.at(i).wavelength < 700e-9);
            CHECK(photons.at(i).direction().z < 0);
            CHECK(fabs(photons.at(i).direction().z) > 0.7);
        }
    }
}
