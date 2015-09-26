#include <iostream> 
#include <string>
#include <math.h>
#include "Tools/AsciiIo.h"

#include "gtest/gtest.h"
#include "Core/PhotonBunch.h"

class PhotonBunchTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(PhotonBunchTest, raw_row2photon) {

    Vector3D dir(66.6, 57.8, 99.9);
    dir.normalize();
    
    std::vector<double> raw_row = {
    // id    sx    sy    sz    dx       dy       dz       wavelength
        0.0, 13.0, 37.0, 42.0, dir.x(), dir.y(), dir.z(), 433.5
    };
    
    Photon* ph = PhotonBunch::raw_row2photon(raw_row);

    EXPECT_EQ(ph->get_id(), raw_row[0]);
    EXPECT_EQ(ph->Support().x(), raw_row[1]);
    EXPECT_EQ(ph->Support().y(), raw_row[2]);
    EXPECT_EQ(ph->Support().z(), raw_row[3]);
    EXPECT_NEAR(ph->Direction().x(), raw_row[4], 1e-9);
    EXPECT_NEAR(ph->Direction().y(), raw_row[5], 1e-9);
    EXPECT_NEAR(ph->Direction().z(), raw_row[6], 1e-9);
    EXPECT_EQ(ph->get_wavelength(), raw_row[7]);
}
//------------------------------------------------------------------------------
TEST_F(PhotonBunchTest, photon2raw_row) {

    Vector3D sup(13.0, 37.0, 42.0);
    Vector3D dir(66.6, 57.8, 99.9);
    uint id = 1337;
    double wavelength = 433.5;

    Photon* ph = new Photon(sup, dir, wavelength);
    ph->set_id(id);
    
    std::vector<double> raw_row = PhotonBunch::photon2raw_row(ph);

    ASSERT_EQ(8, raw_row.size());

    EXPECT_EQ(ph->get_id(), raw_row[0]);
    EXPECT_EQ(ph->Support().x(), raw_row[1]);
    EXPECT_EQ(ph->Support().y(), raw_row[2]);
    EXPECT_EQ(ph->Support().z(), raw_row[3]);
    EXPECT_NEAR(ph->Direction().x(), raw_row[4], 1e-9);
    EXPECT_NEAR(ph->Direction().y(), raw_row[5], 1e-9);
    EXPECT_NEAR(ph->Direction().z(), raw_row[6], 1e-9);
    EXPECT_EQ(ph->get_wavelength(), raw_row[7]);
}
//------------------------------------------------------------------------------
TEST_F(PhotonBunchTest, bunch2raw_matrix2bunch) {

    int number_of_photons = 1e3;

    std::vector<Photon*>* photon_bunch = new std::vector<Photon*>;

    PseudoRandomNumberGenerator prng(0);
    for(int n=0; n<number_of_photons; n++) {

        Vector3D sup(prng.uniform(), prng.uniform(), prng.uniform());
        Vector3D dir(prng.uniform(), prng.uniform(), prng.uniform());
        uint id = int(prng.uniform());
        double wavelength = prng.uniform();

        Photon* ph = new Photon(sup, dir, wavelength);
        ph->set_id(id);

        photon_bunch->push_back(ph);
    }

    std::vector<std::vector<double>> raw_matrix = 
        PhotonBunch::photons2raw_matrix(photon_bunch);

    ASSERT_EQ(number_of_photons, raw_matrix.size());

    std::vector<Photon*> *photon_bunch2 = 
        PhotonBunch::raw_matrix2photons(raw_matrix);

    ASSERT_EQ(number_of_photons, photon_bunch2->size());

    for(int n=0; n<number_of_photons; n++) {

        Photon* ph1 = photon_bunch->at(n);
        Photon* ph2 = photon_bunch2->at(n);

        EXPECT_EQ(ph2->get_id(), ph1->get_id());
        EXPECT_EQ(ph2->Support().x(), ph1->Support().x());
        EXPECT_EQ(ph2->Support().y(), ph1->Support().y());
        EXPECT_EQ(ph2->Support().z(), ph1->Support().z());
        EXPECT_NEAR(ph2->Direction().x(), ph1->Direction().x(), 1e-9);
        EXPECT_NEAR(ph2->Direction().y(), ph1->Direction().y(), 1e-9);
        EXPECT_NEAR(ph2->Direction().z(), ph1->Direction().z(), 1e-9);
        EXPECT_EQ(ph2->get_wavelength(), ph1->get_wavelength());
    }
}
//------------------------------------------------------------------------------
TEST_F(PhotonBunchTest, bunch2raw_matrix2file) {

    int number_of_photons = 1e3;

    std::vector<Photon*>* photon_bunch1 = new std::vector<Photon*>;

    PseudoRandomNumberGenerator prng(0);
    for(int n=0; n<number_of_photons; n++) {

        Vector3D sup(prng.uniform(), prng.uniform(), prng.uniform());
        Vector3D dir(prng.uniform(), prng.uniform(), prng.uniform());
        uint id = int(prng.uniform()*number_of_photons);
        double wavelength = prng.uniform();

        Photon* ph = new Photon(sup, dir, wavelength);
        ph->set_id(id);

        photon_bunch1->push_back(ph);
    }

    // write to text file
    AsciiIo::write_table_to_file(
        PhotonBunch::photons2raw_matrix(photon_bunch1),
        "numeric_table_IO/my_big_photon_list.txt"
    );

    // read back again from text file
    std::vector<Photon*> *photon_bunch2 = PhotonBunch::raw_matrix2photons(
        AsciiIo::gen_table_from_file("numeric_table_IO/my_big_photon_list.txt")
    );  

    ASSERT_EQ(number_of_photons, photon_bunch2->size());

    for(int n=0; n<number_of_photons; n++) {

        Photon* ph1 = photon_bunch1->at(n);
        Photon* ph2 = photon_bunch2->at(n);

        EXPECT_EQ(ph2->get_id(), ph1->get_id());
        EXPECT_NEAR(ph2->Support().x(), ph1->Support().x(), 1e-9);
        EXPECT_NEAR(ph2->Support().y(), ph1->Support().y(), 1e-9);
        EXPECT_NEAR(ph2->Support().z(), ph1->Support().z(), 1e-9);
        EXPECT_NEAR(ph2->Direction().x(), ph1->Direction().x(), 1e-9);
        EXPECT_NEAR(ph2->Direction().y(), ph1->Direction().y(), 1e-9);
        EXPECT_NEAR(ph2->Direction().z(), ph1->Direction().z(), 1e-9);
        EXPECT_NEAR(ph2->get_wavelength(), ph1->get_wavelength(), 1e-9);
    }
}