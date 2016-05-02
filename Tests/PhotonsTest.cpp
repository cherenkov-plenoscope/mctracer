#include "gtest/gtest.h"
#include "Tools/AsciiIo.h"
#include "Core/Photons.h"

class PhotonsTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(PhotonsTest, raw_row2photon) {

    Vec3 dir(66.6, 57.8, 99.9);
    dir.normalize();
    
    std::vector<double> raw_row = {
    // id    sx    sy    sz    dx       dy       dz       wavelength
        0.0, 13.0, 37.0, 42.0, dir.x(), dir.y(), dir.z(), 433.5
    };
    
    Photon* ph = Photons::raw_row2photon(raw_row);

    EXPECT_EQ(ph->get_id(), raw_row[0]);
    EXPECT_EQ(ph->get_support().x(), raw_row[1]);
    EXPECT_EQ(ph->get_support().y(), raw_row[2]);
    EXPECT_EQ(ph->get_support().z(), raw_row[3]);
    EXPECT_NEAR(ph->get_direction().x(), raw_row[4], 1e-9);
    EXPECT_NEAR(ph->get_direction().y(), raw_row[5], 1e-9);
    EXPECT_NEAR(ph->get_direction().z(), raw_row[6], 1e-9);
    EXPECT_EQ(ph->get_wavelength(), raw_row[7]);
}
//------------------------------------------------------------------------------
TEST_F(PhotonsTest, photon2raw_row) {

    Vec3 sup(13.0, 37.0, 42.0);
    Vec3 dir(66.6, 57.8, 99.9);
    uint id = 1337;
    double wavelength = 433.5;

    Photon* ph = new Photon(sup, dir, wavelength);
    ph->set_id(id);
    
    std::vector<double> raw_row = Photons::photon2raw_row(ph);

    ASSERT_EQ(8, raw_row.size());

    EXPECT_EQ(ph->get_id(), raw_row[0]);
    EXPECT_EQ(ph->get_support().x(), raw_row[1]);
    EXPECT_EQ(ph->get_support().y(), raw_row[2]);
    EXPECT_EQ(ph->get_support().z(), raw_row[3]);
    EXPECT_NEAR(ph->get_direction().x(), raw_row[4], 1e-9);
    EXPECT_NEAR(ph->get_direction().y(), raw_row[5], 1e-9);
    EXPECT_NEAR(ph->get_direction().z(), raw_row[6], 1e-9);
    EXPECT_EQ(ph->get_wavelength(), raw_row[7]);
}
//------------------------------------------------------------------------------
TEST_F(PhotonsTest, bunch2raw_matrix2bunch) {

    int number_of_photons = 1e3;

    std::vector<Photon*>* photon_bunch = new std::vector<Photon*>;

    Random::Mt19937 prng(Random::zero_seed);
    for(int n=0; n<number_of_photons; n++) {

        Vec3 sup(prng.uniform(), prng.uniform(), prng.uniform());
        Vec3 dir(prng.uniform(), prng.uniform(), prng.uniform());
        uint id = int(prng.uniform());
        double wavelength = prng.uniform();

        Photon* ph = new Photon(sup, dir, wavelength);
        ph->set_id(id);

        photon_bunch->push_back(ph);
    }

    std::vector<std::vector<double>> raw_matrix = 
        Photons::photons2raw_matrix(photon_bunch);

    ASSERT_EQ(number_of_photons, raw_matrix.size());

    std::vector<Photon*> *photon_bunch2 = 
        Photons::raw_matrix2photons(raw_matrix);

    ASSERT_EQ(number_of_photons, photon_bunch2->size());

    for(int n=0; n<number_of_photons; n++) {

        Photon* ph1 = photon_bunch->at(n);
        Photon* ph2 = photon_bunch2->at(n);

        EXPECT_EQ(ph2->get_id(), ph1->get_id());
        EXPECT_EQ(ph2->get_support().x(), ph1->get_support().x());
        EXPECT_EQ(ph2->get_support().y(), ph1->get_support().y());
        EXPECT_EQ(ph2->get_support().z(), ph1->get_support().z());
        EXPECT_NEAR(ph2->get_direction().x(), ph1->get_direction().x(), 1e-9);
        EXPECT_NEAR(ph2->get_direction().y(), ph1->get_direction().y(), 1e-9);
        EXPECT_NEAR(ph2->get_direction().z(), ph1->get_direction().z(), 1e-9);
        EXPECT_EQ(ph2->get_wavelength(), ph1->get_wavelength());
    }
}
//------------------------------------------------------------------------------
TEST_F(PhotonsTest, bunch2raw_matrix2file) {

    int number_of_photons = 1e3;

    std::vector<Photon*>* photon_bunch1 = new std::vector<Photon*>;

    Random::Mt19937 prng(Random::zero_seed);
    for(int n=0; n<number_of_photons; n++) {

        Vec3 sup(prng.uniform(), prng.uniform(), prng.uniform());
        Vec3 dir(prng.uniform(), prng.uniform(), prng.uniform());
        uint id = int(prng.uniform()*number_of_photons);
        double wavelength = prng.uniform();

        Photon* ph = new Photon(sup, dir, wavelength);
        ph->set_id(id);

        photon_bunch1->push_back(ph);
    }

    // write to text file
    AsciiIo::write_table_to_file(
        Photons::photons2raw_matrix(photon_bunch1),
        "numeric_table_IO/my_big_photon_list.txt"
    );

    // read back again from text file
    std::vector<Photon*> *photon_bunch2 = Photons::raw_matrix2photons(
        AsciiIo::gen_table_from_file("numeric_table_IO/my_big_photon_list.txt")
    );  

    ASSERT_EQ(number_of_photons, photon_bunch2->size());

    for(int n=0; n<number_of_photons; n++) {

        Photon* ph1 = photon_bunch1->at(n);
        Photon* ph2 = photon_bunch2->at(n);

        EXPECT_EQ(ph2->get_id(), ph1->get_id());
        EXPECT_NEAR(ph2->get_support().x(), ph1->get_support().x(), 1e-9);
        EXPECT_NEAR(ph2->get_support().y(), ph1->get_support().y(), 1e-9);
        EXPECT_NEAR(ph2->get_support().z(), ph1->get_support().z(), 1e-9);
        EXPECT_NEAR(ph2->get_direction().x(), ph1->get_direction().x(), 1e-9);
        EXPECT_NEAR(ph2->get_direction().y(), ph1->get_direction().y(), 1e-9);
        EXPECT_NEAR(ph2->get_direction().z(), ph1->get_direction().z(), 1e-9);
        EXPECT_NEAR(ph2->get_wavelength(), ph1->get_wavelength(), 1e-9);
    }
}