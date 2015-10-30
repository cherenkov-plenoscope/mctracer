#include <iostream> 
#include <string>
#include <math.h>
#include <algorithm> 
#include <iterator>
#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include "MmcsCorsikaFileIO/EventIo.h"

using namespace EventIo;

// The fixture for testing class Foo.
class EventIoTest : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if its body
    // is empty.
    
    EventIoTest() {
        // You can do set-up work for each test here.
    }

    virtual ~EventIoTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Foo.
};
//----------------------------------------------------------------------
TEST_F(EventIoTest, EventIoHeader_works) {
    
    //std::stringstream fake_file;
    std::ifstream fake_file;
    fake_file.open("telescope.dat");
    /*
    for (size_t i=0; i != telescope_dat.size(); i++){
        fake_file << telescope_dat[i];
    }
    */

    EventIo::Header my_header(fake_file);
    
    EXPECT_TRUE(my_header.is_sync);
    EXPECT_EQ(1200, my_header.type);
    EXPECT_EQ(0, my_header.version);
    EXPECT_FALSE(my_header.user);
    EXPECT_FALSE(my_header.extended);
    EXPECT_FALSE(my_header.only_sub_objects);
    EXPECT_EQ(1096, my_header.length);
    EXPECT_EQ(7, my_header.id);
    EXPECT_EQ(3, 3.);

}

TEST_F(EventIoTest,  EventIoHeader_fails_wrong_sync_marker) {

    std::ifstream fin("telescope.dat");
    stringstream sout;

    std::copy_n(istreambuf_iterator<char>(fin), 100,  ostreambuf_iterator<char>(sout));

    // Here  destroy something in the sync word.
    sout.seekp(2);
    sout.put(0x00);
    sout.seekp(0);

    
    EXPECT_THROW(EventIo::Header my_header(sout), TracerException);
    
}

TEST_F(EventIoTest, EventIoHeader_fails_empty_file) {
    std::stringstream fake_file;
    EXPECT_THROW(EventIo::Header my_header(fake_file), TracerException);
}



TEST_F(EventIoTest, make_runheader) {
    ifstream fin("telescope.dat");
    stringstream sout;

    auto foo = istreambuf_iterator<char>(fin);
    for (size_t i=0; i<16; i++) foo++;

    std::copy_n(foo, 100,  ostreambuf_iterator<char>(sout));
    
    MmcsCorsikaRunHeader my_run_header = EventIo::make_run_header_from_stream(sout);
}


TEST_F(EventIoTest, EventIoFile_telescope_dat__check_tel_pos) {
    EventIoFile my_file("telescope.dat");
    EXPECT_EQ(1, my_file.run_header.tel_pos.size());
    EXPECT_EQ(0., my_file.run_header.tel_pos[0].x);
    EXPECT_EQ(0., my_file.run_header.tel_pos[0].y);
    EXPECT_EQ(500., my_file.run_header.tel_pos[0].z);
    EXPECT_EQ(500., my_file.run_header.tel_pos[0].r);
}

TEST_F(EventIoTest, EventIoFile_telescope_dat__check_input_card) {
    EventIoFile my_file("telescope.dat");
    EXPECT_EQ(' ', my_file.run_header.input_card[100]);
}

TEST_F(EventIoTest, EventIoFile_telescope_dat__mmcs_run_header) {
    EventIoFile my_file("telescope.dat");
    EXPECT_NEAR(7., my_file.run_header.mmcs_runheader.run_number, 1e-6);
    EXPECT_NEAR(-2.7, my_file.run_header.mmcs_runheader.slope_of_energy_spektrum, 1e-6);
    EXPECT_EQ(2, my_file.run_header.mmcs_runheader.energy_range.size());
    EXPECT_NEAR(1000., my_file.run_header.mmcs_runheader.energy_range[0], 1e-6);
    EXPECT_NEAR(50000., my_file.run_header.mmcs_runheader.energy_range[1], 1e-6);

    EXPECT_EQ(1, my_file.run_header.mmcs_runheader.observation_levels.size());
    EXPECT_NEAR(220000., my_file.run_header.mmcs_runheader.observation_levels[0], 1e-6 );  
}

TEST_F(EventIoTest, EventIoFile_telescope_dat__next_call) {
    EventIoFile my_file("telescope.dat");
    std::vector< std::vector< float > > photon_bundle = my_file.next();
}

TEST_F(EventIoTest, EventIoFile_telescope_dat__event_header) {
    EventIoFile my_file("telescope.dat");
    std::vector< std::vector< float > > photon_bundle = my_file.next();

    EXPECT_EQ(1, my_file.current_event_header.telescope_offsets.size());
    EXPECT_NEAR(379489.3125, my_file.current_event_header.telescope_offsets[0].toff, 1e-6);
    //std::cout << my_file.current_event_header.telescope_offsets[0].get_print() << std::endl;
    EXPECT_NEAR(-0., my_file.current_event_header.telescope_offsets[0].xoff, 1e-6);
    EXPECT_NEAR(-6589.96044922, my_file.current_event_header.telescope_offsets[0].yoff, 1e-6);

    MmcsCorsikaEventHeader h = my_file.current_event_header.mmcs_event_header;
    EXPECT_NEAR(1. ,h.event_number, 1e-6);
    EXPECT_NEAR(1. ,h.particle_id, 1e-6);
    EXPECT_NEAR(1. ,h.particle_id, 1e-6);
    EXPECT_NEAR(2745.3125 ,h.total_energy_in_GeV, 1e-6);
}


TEST_F(EventIoTest, EventIoFile_telescope_dat__photon_bundle_size) {
    EventIoFile my_file("telescope.dat");
    std::vector< std::vector< float > > photon_bundle = my_file.next();
    EXPECT_EQ(42629, photon_bundle.size());

    for (size_t i=0; i<photon_bundle.size(); i++)
    {
        std::vector<float> photon_bunch = photon_bundle[i];
        EXPECT_EQ(8, photon_bunch.size());    
    }
}

TEST_F(EventIoTest, EventIoFile_telescope_dat__photon_bundle_size2) {
    EventIoFile my_file("telescope.dat");
    std::vector< std::vector< float > > photon_bundle = my_file.next();
    EXPECT_EQ(42629, photon_bundle.size());

    for (size_t i=0; i<photon_bundle.size(); i++)
    {
        std::vector<float> photon_bunch = photon_bundle[i];
        EXPECT_EQ(8, photon_bunch.size());    
    }
}

TEST_F(EventIoTest, EventIoFile_telescope_dat__photon_bundle_values) {
    EventIoFile my_file("telescope.dat");
    std::vector< std::vector< float > > photon_bundle = my_file.next();
    EXPECT_EQ(42629, photon_bundle.size());

float some_photon_bundles[5][8] = {
    {161.90000915527344, 236.8000030517578, 0.23186667263507843, 0.01769999973475933, -11.800000190734863, 1462178.625, 1.0, -692.0},
    {-323.5, 373.3000183105469, 0.23149999976158142, 0.017799999564886093, -15.40000057220459, 1458816.0, 1.0, -380.0},
    {-1.899999976158142, 286.20001220703125, 0.23173333704471588, 0.017733333632349968, -13.0, 1458816.0, 1.0, -351.0},
    {43.5, 483.0, 0.23313333094120026, 0.0351666659116745, -10.199999809265137, 1276440.0, 0.9899999499320984, -373.0},
    {7.300000190734863, -457.1000061035156, 0.23346666991710663, 0.03263333439826965, -11.699999809265137, 1358314.625, 0.9899999499320984, -523.0}
};

 for (int j=2; j<5; j++)
 {
    std::vector<float> photon_bunch = photon_bundle[j];
    
    EXPECT_FLOAT_EQ(some_photon_bundles[j][0], photon_bunch[0]);
    EXPECT_FLOAT_EQ(some_photon_bundles[j][1], photon_bunch[1]);
    EXPECT_FLOAT_EQ(some_photon_bundles[j][2], photon_bunch[2]);
    EXPECT_FLOAT_EQ(some_photon_bundles[j][3], photon_bunch[3]);
    EXPECT_FLOAT_EQ(some_photon_bundles[j][4], photon_bunch[4]);
    EXPECT_NEAR(some_photon_bundles[j][5], photon_bunch[5], 10); // the height can sometimes be way off!
    EXPECT_FLOAT_EQ(some_photon_bundles[j][6], photon_bunch[6]);
    EXPECT_FLOAT_EQ(some_photon_bundles[j][7], photon_bunch[7]);

 }
}


TEST_F(EventIoTest, EventIoFile_telescope_dat__run_time____________________________________) {
    EventIoFile my_file("telescope.dat");
    while (my_file.has_still_events_left())
    {
        my_file.next();
    }
}

