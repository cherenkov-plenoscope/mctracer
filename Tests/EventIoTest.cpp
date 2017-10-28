#include "gtest/gtest.h"
#include "Corsika/EventIo/EventIo.h"
#include "Corsika/Tools.h"
#include <algorithm>

class EventIoTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(EventIoTest, EventIoHeader_works) {
    
    std::ifstream fake_file;
    fake_file.open("telescope.dat");
    EventIo::Header my_header(fake_file);
    
    EXPECT_TRUE(my_header.is_sync);
    EXPECT_EQ(1200, my_header.type);
    EXPECT_EQ(0, my_header.version);
    EXPECT_FALSE(my_header.user);
    EXPECT_FALSE(my_header.extended);
    EXPECT_FALSE(my_header.only_sub_objects);
    EXPECT_EQ(1096u, my_header.length);
    EXPECT_EQ(7, my_header.id);
}
//------------------------------------------------------------------------------
TEST_F(EventIoTest,  EventIoHeader_fails_wrong_sync_marker) {

    std::ifstream fin("telescope.dat");
    std::stringstream sout;

    std::copy_n(std::istreambuf_iterator<char>(fin), 100, std::ostreambuf_iterator<char>(sout));

    // Here  destroy something in the sync word.
    sout.seekp(2);
    sout.put(0x00);
    sout.seekp(0);
    
    EXPECT_THROW(EventIo::Header my_header(sout), EventIo::NoSyncFoundException);
}
//------------------------------------------------------------------------------
TEST_F(EventIoTest, EventIoHeader_fails_empty_file) {
    std::stringstream fake_file;
    EXPECT_THROW(EventIo::Header my_header(fake_file), std::runtime_error);
}
//------------------------------------------------------------------------------
TEST_F(EventIoTest, make_runheader) {

    std::ifstream fin("telescope.dat");
    std::stringstream sout;

    auto foo = std::istreambuf_iterator<char>(fin);
    for (size_t i=0; i<16; i++) foo++;

    std::copy_n(foo, 100, std::ostreambuf_iterator<char>(sout));
    array<float, 273> my_run_header = 
        EventIo::make_corsika_273float_sub_block_form_stream(sout);

    EXPECT_EQ(Corsika::str2float("RUNH"), my_run_header.at(0));
}
//------------------------------------------------------------------------------
TEST_F(EventIoTest, EventIoFile_telescope_dat__check_tel_pos) {

    EventIo::Run my_run("telescope.dat");
    EXPECT_EQ(1u, my_run.header.tel_pos.size());
    EXPECT_EQ(0., my_run.header.tel_pos[0].x);
    EXPECT_EQ(0., my_run.header.tel_pos[0].y);
    EXPECT_EQ(500., my_run.header.tel_pos[0].z);
    EXPECT_EQ(500., my_run.header.tel_pos[0].r);
}
//------------------------------------------------------------------------------
TEST_F(EventIoTest, EventIoFile_telescope_dat__check_input_card) {

    EventIo::Run my_run("telescope.dat");
    EXPECT_EQ(' ', my_run.header.input_card[100]);
}
//------------------------------------------------------------------------------
TEST_F(EventIoTest, EventIoFile_telescope_dat__mmcs_run_header) {
    EventIo::Run my_run("telescope.dat");
    EXPECT_NEAR(7., Corsika::RunHeader::run_number(my_run.header.raw), 1e-6);
    EXPECT_NEAR(-2.7, Corsika::RunHeader::slope_of_energy_spektrum(my_run.header.raw), 1e-6);
    EXPECT_NEAR(1000., Corsika::RunHeader::energy_range_start(my_run.header.raw), 1e-6);
    EXPECT_NEAR(50000., Corsika::RunHeader::energy_range_end(my_run.header.raw), 1e-6);

    EXPECT_EQ(1u, Corsika::RunHeader::number_of_observation_levels(my_run.header.raw));
    EXPECT_NEAR(220000., Corsika::RunHeader::observation_level_at(my_run.header.raw, 0), 1e-6);  
}
//------------------------------------------------------------------------------
TEST_F(EventIoTest, EventIoFile_telescope_dat__next_call) {

    EventIo::Run my_run("telescope.dat");
    EventIo::Event event = my_run.next_event();
}
//------------------------------------------------------------------------------
TEST_F(EventIoTest, EventIoFile_telescope_dat__event_header) {

    EventIo::Run my_run("telescope.dat");
    EventIo::Event event = my_run.next_event();

    EXPECT_EQ(1u, event.header.telescope_offsets.size());
    EXPECT_NEAR(379489.3125, event.header.telescope_offsets[0].toff, 1e-6);
    //std::cout << my_run.current_event_header.telescope_offsets[0].str() << std::endl;
    EXPECT_NEAR(-0., event.header.telescope_offsets[0].xoff, 1e-6);
    EXPECT_NEAR(-6589.96044922, event.header.telescope_offsets[0].yoff, 1e-6);

    array<float, 273> h = event.header.raw;
    EXPECT_NEAR(1., Corsika::EventHeader::event_number(h), 1e-6);
    EXPECT_NEAR(1., Corsika::EventHeader::particle_id(h), 1e-6);
    EXPECT_NEAR(2745.3125, Corsika::EventHeader::total_energy_in_GeV(h), 1e-6);
}
//------------------------------------------------------------------------------
TEST_F(EventIoTest, EventIoFile_telescope_dat__photon_bundle_size) {

    EventIo::Run my_run("telescope.dat");
    EventIo::Event event = my_run.next_event();
    EXPECT_EQ(42629u, event.photons.size());

    for (size_t i=0; i<event.photons.size(); i++)
    {
        std::array<float, 8> photon_bunch = event.photons[i];
        EXPECT_EQ(8u, photon_bunch.size());    
    }
}
//------------------------------------------------------------------------------
TEST_F(EventIoTest, EventIoFile_telescope_dat__photon_bundle_values) {

    EventIo::Run my_run("telescope.dat");
    EventIo::Event event = my_run.next_event();
    EXPECT_EQ(42629u, event.photons.size());

    float some_photon_bundles[5][8] = {
        {161.90000915527344, 236.8000030517578, 0.23186667263507843, 0.01769999973475933, -11.800000190734863, 1462178.625, 1.0, -692.0},
        {-323.5, 373.3000183105469, 0.23149999976158142, 0.017799999564886093, -15.40000057220459, 1458816.0, 1.0, -380.0},
        {-1.899999976158142, 286.20001220703125, 0.23173333704471588, 0.017733333632349968, -13.0, 1458816.0, 1.0, -351.0},
        {43.5, 483.0, 0.23313333094120026, 0.0351666659116745, -10.199999809265137, 1276440.0, 0.9899999499320984, -373.0},
        {7.300000190734863, -457.1000061035156, 0.23346666991710663, 0.03263333439826965, -11.699999809265137, 1358314.625, 0.9899999499320984, -523.0}
    };

    for (int j=2; j<5; j++) {

        std::array<float, 8> photon_bunch = event.photons[j];

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
//------------------------------------------------------------------------------
#include "Corsika/EventIo/PhotonFactory.h"

TEST_F(EventIoTest, EventIoFile_telescope_dat_run_time) {
    EventIo::Run my_run("telescope.dat");
    
    while (my_run.has_still_events_left()) {

        EventIo::Event event = my_run.next_event();

        Random::Mt19937 prng;

        vector<Photon> photons;
        uint id = 0;
        for(array<float, 8> corsika_photon : event.photons) {
            
            EventIo::PhotonFactory cpf(corsika_photon,id++,&prng);

            if(cpf.passed_atmosphere()) {
                photons.push_back(
                    cpf.get_photon()
                );
            }
            //std::cout << photons.size() << "\n";
            //std::cout << PhotonBunch::get_print(&photons) << "\n";
        }
    }
}

