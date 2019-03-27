// Copyright 2014 Sebastian A. Mueller, Dominik Neise
#include <algorithm>
#include <sstream>
#include "catch.hpp"
#include "merlict_corsika/eventio.h"
#include "merlict_corsika/corsika.h"
#include "merlict_corsika/PhotonFactory.h"


TEST_CASE("EventIoTest: EventIoHeader_works", "[merlict]") {
    std::ifstream fake_file;
    fake_file.open("tests/resources/telescope.dat");
    eventio::Header my_header(fake_file);
    CHECK(my_header.is_sync);
    CHECK(my_header.type == 1200);
    CHECK(my_header.version == 0);
    CHECK(!my_header.user);
    CHECK(!my_header.extended);
    CHECK(!my_header.only_sub_objects);
    CHECK(my_header.length == 1096u);
    CHECK(my_header.id == 7);
}

TEST_CASE("EventIoTest: EventIoHeader_fails_wrong_sync_marker", "[merlict]") {
    std::ifstream fin("tests/resources/telescope.dat");
    std::stringstream sout;
    std::copy_n(
        std::istreambuf_iterator<char>(fin),
        100,
        std::ostreambuf_iterator<char>(sout));
    // Here  destroy something in the sync word.
    sout.seekp(2);
    sout.put(0x00);
    sout.seekp(0);
    CHECK_THROWS_AS(eventio::Header(sout), eventio::NoSyncFoundException);
}

TEST_CASE("EventIoTest: EventIoHeader_fails_empty_file", "[merlict]") {
    std::stringstream fake_file;
    CHECK_THROWS_AS(eventio::Header(fake_file), std::runtime_error);
}

TEST_CASE("EventIoTest: make_runheader", "[merlict]") {
    std::ifstream fin("tests/resources/telescope.dat");
    std::stringstream sout;

    auto foo = std::istreambuf_iterator<char>(fin);
    for (size_t i = 0; i < 16; i++) foo++;

    std::copy_n(foo, 100, std::ostreambuf_iterator<char>(sout));
    std::array<float, 273> my_run_header =
        eventio::make_corsika_273float_sub_block_form_stream(sout);

    CHECK(my_run_header.at(0) == corsika::str2float("RUNH"));
}

TEST_CASE("EventIoTest: EventIoFile_telescope_dat__check_tel_pos", "[merlict]") {
    eventio::Run my_run("tests/resources/telescope.dat");
    CHECK(my_run.header.tel_pos.size() == 1u);
    CHECK(my_run.header.tel_pos[0].x == 0.);
    CHECK(my_run.header.tel_pos[0].y == 0.);
    CHECK(my_run.header.tel_pos[0].z == 500.);
    CHECK(my_run.header.tel_pos[0].r == 500.);
}

TEST_CASE("EventIoTest: EventIoFile_telescope_dat__check_input_card", "[merlict]") {
    eventio::Run my_run("tests/resources/telescope.dat");
    CHECK(my_run.header.input_card[100] == ' ');
}

TEST_CASE("EventIoTest: EventIoFile_telescope_dat__mmcs_run_header", "[merlict]") {
    eventio::Run my_run("tests/resources/telescope.dat");
    CHECK(7. == Approx(corsika::header::run::run_number(my_run.header.raw)).margin(1e-6));
    CHECK(-2.7 == Approx(corsika::header::run::slope_of_energy_spektrum(my_run.header.raw)).margin(1e-6));
    CHECK(1000. == Approx(corsika::header::run::energy_range_start(my_run.header.raw)).margin(1e-6));
    CHECK(50000. == Approx(corsika::header::run::energy_range_end(my_run.header.raw)).margin(1e-6));
    CHECK(corsika::header::run::num_observation_levels(my_run.header.raw) == 1u);
    CHECK(220000. == Approx(corsika::header::run::observation_level_at(my_run.header.raw, 0)).margin(1e-6));
}

TEST_CASE("EventIoTest: EventIoFile_telescope_dat__next_call", "[merlict]") {
    eventio::Run my_run("tests/resources/telescope.dat");
    eventio::Event event = my_run.next_event();
}

TEST_CASE("EventIoTest: EventIoFile_telescope_dat__event_header", "[merlict]") {
    eventio::Run my_run("tests/resources/telescope.dat");
    eventio::Event event = my_run.next_event();

    CHECK(event.header.telescope_offsets.size() == 1u);
    CHECK(379489.3125 == Approx(event.header.telescope_offsets[0].toff).margin(1e-6));
    // std::cout << my_run.current_event_header.telescope_offsets[0].str();
    // std::cout << std::endl;
    CHECK(-0. == Approx(event.header.telescope_offsets[0].xoff).margin(1e-6));
    CHECK(-6589.96044922 == Approx(event.header.telescope_offsets[0].yoff).margin(1e-6));

    std::array<float, 273> h = event.header.raw;
    CHECK(1. == Approx(corsika::header::event::event_number(h)).margin(1e-6));
    CHECK(1. == Approx(corsika::header::event::particle_id(h)).margin(1e-6));
    CHECK(2745.3125 == Approx(corsika::header::event::total_energy_in_GeV(h)).margin(1e-6));
}

TEST_CASE("EventIoTest: EventIoFile_telescope_dat__photon_bundle_size", "[merlict]") {
    eventio::Run my_run("tests/resources/telescope.dat");
    eventio::Event event = my_run.next_event();
    CHECK(event.photons.size() == 42629u);

    for (size_t i = 0; i < event.photons.size(); i++) {
        std::array<float, 8> photon_bunch = event.photons[i];
        CHECK(photon_bunch.size() == 8u);
    }
}

TEST_CASE("EventIoTest: EventIoFile_telescope_dat__photon_bundle_values", "[merlict]") {
    eventio::Run my_run("tests/resources/telescope.dat");
    eventio::Event event = my_run.next_event();
    CHECK(event.photons.size() == 42629u);

    float some_photon_bundles[5][8] = {
        {
            161.90000915527344,
            236.8000030517578,
            0.23186667263507843,
            0.01769999973475933,
            -11.800000190734863,
            1462178.625,
            1.0,
            -692.0
        },
        {
            -323.5,
            373.3000183105469,
            0.23149999976158142,
            0.017799999564886093,
            -15.40000057220459,
            1458816.0,
            1.0,
            -380.0
        },
        {
            -1.899999976158142,
            286.20001220703125,
            0.23173333704471588,
            0.017733333632349968,
            -13.0,
            1458816.0,
            1.0,
            -351.0
        },
        {
            43.5,
            483.0,
            0.23313333094120026,
            0.0351666659116745,
            -10.199999809265137,
            1276440.0,
            0.9899999499320984,
            -373.0
        },
        {
            7.300000190734863,
            -457.1000061035156,
            0.23346666991710663,
            0.03263333439826965,
            -11.699999809265137,
            1358314.625,
            0.9899999499320984,
            -523.0
        }
    };

    for (int j = 2; j < 5; j++) {
        std::array<float, 8> photon_bunch = event.photons[j];
        CHECK(photon_bunch[0] == Approx(some_photon_bundles[j][0]));
        CHECK(photon_bunch[1] == Approx(some_photon_bundles[j][1]));
        CHECK(photon_bunch[2] == Approx(some_photon_bundles[j][2]));
        CHECK(photon_bunch[3] == Approx(some_photon_bundles[j][3]));
        CHECK(photon_bunch[4] == Approx(some_photon_bundles[j][4]));
        CHECK(some_photon_bundles[j][5] == Approx(photon_bunch[5]).margin(10));
        // the height can sometimes be way off!
        CHECK(photon_bunch[6] == Approx(some_photon_bundles[j][6]));
        CHECK(photon_bunch[7] == Approx(some_photon_bundles[j][7]));
    }
}

TEST_CASE("EventIoTest: EventIoFile_telescope_dat_run_time", "[merlict]") {
    eventio::Run my_run("tests/resources/telescope.dat");

    while (my_run.has_still_events_left()) {
        eventio::Event event = my_run.next_event();

        merlict::random::Mt19937 prng;

        std::vector<merlict::Photon> photons;
        unsigned int id = 0;
        for (std::array<float, 8> corsika_photon : event.photons) {
            merlict::EventIoPhotonFactory cpf(corsika_photon, id++, &prng);

            if (cpf.passed_atmosphere()) {
                photons.push_back(cpf.make_photon());
            }
        }
    }
}

