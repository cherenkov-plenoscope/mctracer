// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;


TEST_CASE("PhotonsTest: raw_row2photon", "[merlict]") {
    ml::Vec3 dir(66.6, 57.8, 99.9);
    dir.normalize();

    std::vector<double> raw_row = {
    // id    sx    sy    sz    dx     dy     dz     wavelength
        0.0, 13.0, 37.0, 42.0, dir.x, dir.y, dir.z, 433.5
    };

    ml::Photon ph = ml::raw_row2photon(raw_row);

    CHECK(raw_row[0] == ph.simulation_truth_id);
    CHECK(raw_row[1] == ph.support().x);
    CHECK(raw_row[2] == ph.support().y);
    CHECK(raw_row[3] == ph.support().z);
    CHECK(ph.direction().x == Approx(raw_row[4]).margin(1e-9));
    CHECK(ph.direction().y == Approx(raw_row[5]).margin(1e-9));
    CHECK(ph.direction().z == Approx(raw_row[6]).margin(1e-9));
    CHECK(raw_row[7] == ph.get_wavelength());
}

TEST_CASE("PhotonsTest: photon2raw_row", "[merlict]") {
    ml::Vec3 sup(13.0, 37.0, 42.0);
    ml::Vec3 dir(66.6, 57.8, 99.9);
    unsigned int id = 1337;
    double wavelength = 433.5;

    ml::Photon ph(sup, dir, wavelength);
    ph.simulation_truth_id = id;

    std::vector<double> raw_row = ml::photon2raw_row(&ph);

    REQUIRE(raw_row.size() == 8u);

    CHECK(raw_row[0] == ph.simulation_truth_id);
    CHECK(raw_row[1] == ph.support().x);
    CHECK(raw_row[2] == ph.support().y);
    CHECK(raw_row[3] == ph.support().z);
    CHECK(ph.direction().x == Approx(raw_row[4]).margin(1e-9));
    CHECK(ph.direction().y == Approx(raw_row[5]).margin(1e-9));
    CHECK(ph.direction().z == Approx(raw_row[6]).margin(1e-9));
    CHECK(raw_row[7] == ph.get_wavelength());
}

TEST_CASE("PhotonsTest: bunch2raw_matrix2bunch", "[merlict]") {
    const unsigned int num_photons = 1e3;
    std::vector<ml::Photon> photon_bunch;
    ml::random::Mt19937 prng(0u);
    for (unsigned int n = 0; n < num_photons; n++) {
        ml::Vec3 sup(prng.uniform(), prng.uniform(), prng.uniform());
        ml::Vec3 dir(prng.uniform(), prng.uniform(), prng.uniform());
        unsigned int id =  static_cast<int>(prng.uniform());
        double wavelength = prng.uniform();

        ml::Photon ph(sup, dir, wavelength);
        ph.simulation_truth_id = id;
        photon_bunch.push_back(ph);
    }

    std::vector<std::vector<double>> raw_matrix =
        ml::photons2raw_matrix(&photon_bunch);

    REQUIRE(raw_matrix.size() == num_photons);

    std::vector<ml::Photon> photon_bunch2 =
        ml::raw_matrix2photons(raw_matrix);

    REQUIRE(photon_bunch2.size() == num_photons);

    for (unsigned int n = 0; n < num_photons; n++) {
        ml::Photon ph1 = photon_bunch.at(n);
        ml::Photon ph2 = photon_bunch2.at(n);

        CHECK(ph1.simulation_truth_id == ph2.simulation_truth_id);
        CHECK(ph1.support().x == ph2.support().x);
        CHECK(ph1.support().y == ph2.support().y);
        CHECK(ph1.support().z == ph2.support().z);
        CHECK(ph2.direction().x == Approx(ph1.direction().x).margin(1e-9));
        CHECK(ph2.direction().y == Approx(ph1.direction().y).margin(1e-9));
        CHECK(ph2.direction().z == Approx(ph1.direction().z).margin(1e-9));
        CHECK(ph1.get_wavelength() == ph2.get_wavelength());
    }
}

TEST_CASE("PhotonsTest: bunch2raw_matrix2file", "[merlict]") {
    const unsigned int num_photons = 1e3;
    std::vector<ml::Photon> photon_bunch1;
    ml::random::Mt19937 prng(0u);
    for (unsigned int n = 0; n < num_photons; n++) {
        ml::Vec3 sup(prng.uniform(), prng.uniform(), prng.uniform());
        ml::Vec3 dir(prng.uniform(), prng.uniform(), prng.uniform());
        unsigned int id = static_cast<int>(prng.uniform()*num_photons);
        double wavelength = prng.uniform();

        ml::Photon ph(sup, dir, wavelength);
        ph.simulation_truth_id = id;

        photon_bunch1.push_back(ph);
    }

    // write to text file
    ml::tsvio::write_table_to_file(
        ml::photons2raw_matrix(&photon_bunch1),
        "tsvio/my_big_photon_list.txt");

    // read back again from text file
    std::vector<ml::Photon> photon_bunch2 = ml::raw_matrix2photons(
        ml::tsvio::gen_table_from_file(
            "tsvio/my_big_photon_list.txt"));

    REQUIRE(photon_bunch2.size() == num_photons);

    for (unsigned int n = 0; n < num_photons; n++) {
        ml::Photon ph1 = photon_bunch1.at(n);
        ml::Photon ph2 = photon_bunch2.at(n);

        CHECK(ph1.simulation_truth_id == ph2.simulation_truth_id);
        CHECK(ph2.support().x == Approx(ph1.support().x).margin(1e-9));
        CHECK(ph2.support().y == Approx(ph1.support().y).margin(1e-9));
        CHECK(ph2.support().z == Approx(ph1.support().z).margin(1e-9));
        CHECK(ph2.direction().x == Approx(ph1.direction().x).margin(1e-9));
        CHECK(ph2.direction().y == Approx(ph1.direction().y).margin(1e-9));
        CHECK(ph2.direction().z == Approx(ph1.direction().z).margin(1e-9));
        CHECK(ph2.get_wavelength() == Approx(ph1.get_wavelength()).margin(1e-9));
    }
}
