// Copyright 2014 Sebastian A. Mueller
#include <numeric>
#include "gtest/gtest.h"
#include "Corsika/PhotonFactory.h"
#include "eventio.h"
#include "Scenery/primitive/Disc.h"
#include "sensor/PhotonSensor.h"
#include "Core/Photons.h"
using std::vector;
using namespace relleums;

class EventIoPhotonFactoryTest : public ::testing::Test {};

TEST_F(EventIoPhotonFactoryTest, intersection_point_on_ground) {
    // compare the input ground intersection point with the actual intersection
    // point of the mctracer photons when they are absorbed on the ground.
    random::Mt19937 prng(0);

    for (float x = -1e4; x < 1e4; x = x+1495.0) {
        for (float y = -1e4; y < 1e4; y = y+1495.0) {
            for (float cx = -0.5; cx < 0.5; cx = cx+0.11) {
                for (float cy = -0.5; cy < 0.5; cy = cy+0.11) {
                    const std::array<float, 8> corsika_photon =
                    {    x,   y,   cx,  cy,  0.0,  1e5, 1.0,   433};
                    //   x    y    xcos ycos time  zem  weight lambda
                    //   cm   cm   1    1    ns    cm   1      nm
                    const unsigned int id = 1337;
                    random::FakeConstant fake_prng(0.0);

                    EventIoPhotonFactory cpf(corsika_photon, id, &fake_prng);

                    vector<Photon> photons;
                    photons.push_back(cpf.get_photon());

                    // propagate mctracer photons down to ground
                    Frame world;
                    world.set_name_pos_rot("world", VEC3_ORIGIN, ROT3_UNITY);

                    Disc* ground = world.append<Disc>();
                    ground->set_name_pos_rot(
                        "ground",
                        VEC3_ORIGIN,
                        ROT3_UNITY);
                    const Color* ground_color = &COLOR_GRAY;
                    const unsigned int ground_sensor_id = 0;
                    ground->set_outer_color(ground_color);
                    ground->set_inner_color(ground_color);
                    ground->set_radius(1e3);

                    sensor::Sensor sensor(ground_sensor_id, ground);
                    vector<sensor::Sensor*> sensor_vec = {&sensor};
                    sensor::Sensors sensor_list(sensor_vec);

                    world.init_tree_based_on_mother_child_relations();

                    // propagation settings
                    PropagationConfig settings;
                    settings.use_multithread_when_possible = false;

                    // photon propagation down to the ground
                    Photons::propagate_photons_in_scenery_with_settings(
                        &photons, &world, &settings, &prng);

                    // detect photons in ground sensor
                    sensor_list.clear_history();
                    sensor_list.assign_photons(&photons);

                    ASSERT_EQ(1u, sensor.photon_arrival_history.size());

                    EXPECT_NEAR(
                        x*1e-2,
                        sensor.photon_arrival_history[0].x_intersect, 1e-6);
                    EXPECT_NEAR(
                        y*1e-2,
                        sensor.photon_arrival_history[0].y_intersect, 1e-6);
                }
            }
        }
    }
}

TEST_F(EventIoPhotonFactoryTest, wavelength_sign) {
    random::FakeConstant prng(0.0);
    for (int l = 1; l < 1337; l++) {
        const float sign = (l%2 == 0) ? +1.0: -1.0;
        const float lambda = static_cast<float>(l)*sign;
        const std::array<float, 8> corsika_photon = {
             1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.999, lambda};
        //   x    y    xcos ycos time  zem  weight lambda
        //   cm   cm   1    1    ns    cm   1      nm

        const int id = 1337;
        EventIoPhotonFactory cpf(corsika_photon, id, &prng);
        ASSERT_NEAR(fabs(lambda), cpf.wavelength()*1e9, 1e-2);
        ASSERT_TRUE(cpf.wavelength() > 0.0);
    }
}

TEST_F(EventIoPhotonFactoryTest, convert_photons) {
    const std::array<float, 8> corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.999, 433};
    //   x    y    xcos ycos time  zem  weight lambda
    //   cm   cm   1    1    ns    cm   1    nm

    const int id = 1337;
    random::FakeConstant prng(0.0);

    EventIoPhotonFactory cpf(corsika_photon, id, &prng);

    ASSERT_TRUE(cpf.passed_atmosphere());
    Photon ph = cpf.get_photon();

    EXPECT_EQ(id, ph.get_simulation_truth_id());
    EXPECT_NEAR(433e-9, ph.get_wavelength(), 1e-9);

    // since the x,y angles are zero, the support vector can be tested
    EXPECT_NEAR(0.012, ph.support().x, 1e-9);
    EXPECT_NEAR(0.034, ph.support().y, 1e-9);
    EXPECT_NEAR(1e3, ph.support().z, 1e-9);
}

TEST_F(EventIoPhotonFactoryTest, execute_atmospheric_absorption) {
    // In Corsika photons can have a 'weight' to represent a bunch of photons.
    // This was introduced in Corsika to save computations and disk space.
    // Corsika does not recomend to have weights above 5.
    // Mctracer has no concet of weighted photon bunches.
    // We force Corsika to produce single photons, i.e. the maximum weight
    // in the Corsika output is 1.0.
    // A weight below 1.0 means, that the photon passed Corsika's atmosphere
    // simulation with a probability of this weigth.
    // In mctracer we collapse this photons probability when the photon
    // is created in mctracer. This is why the EventIoPhotonFactory is
    // given a PseudoRandomNumberGenerator.

    double absorbed = 0;
    double passed = 0;
    double total = 1e5;

    random::Mt19937 prng(random::ZERO_SEED);

    for (double i = 0; i < total-1; i++) {
        float weight = i/total;  // from 0.0 to 1.0
        const std::array<float, 8> corsika_photon =
            {1.2, 3.4, 0.0, 0.0, 1e-9, 1e3, weight, 433};
        //   x    y    xcos ycos time  zem  weight lambda
        //   cm   cm   1    1    ns    cm   1    nm

        EventIoPhotonFactory cpf(corsika_photon, 1337, &prng);

        if (cpf.passed_atmosphere())
            passed++;
        else
            absorbed++;
    }

    double passed_ratio = passed/total;
    EXPECT_NEAR(0.5, passed_ratio, 1e-2);
}

TEST_F(EventIoPhotonFactoryTest, mctracer_rejects_photon_weight_below_0) {
    const unsigned int id = 1337;
    random::Mt19937 prng(random::ZERO_SEED);
    const std::array<float, 8> corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, -0.1, 433};

    EXPECT_THROW(
        EventIoPhotonFactory cpf(corsika_photon, id, &prng),
        EventIoPhotonFactory::BadPhotonWeight);
}

TEST_F(EventIoPhotonFactoryTest, mctracer_accepts_photon_weight_equal_1) {
    const unsigned int id = 1337;
    random::Mt19937 prng(random::ZERO_SEED);
    const std::array<float, 8> corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 1.0, 433};

    EXPECT_NO_THROW(
        EventIoPhotonFactory cpf(corsika_photon, id, &prng));
}

TEST_F(EventIoPhotonFactoryTest, mctracer_rejects_photon_weight_above_1) {
    const unsigned int id = 1337;
    random::Mt19937 prng(random::ZERO_SEED);
    const std::array<float, 8> corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 1.1, 433};

    EXPECT_THROW(
        EventIoPhotonFactory cpf(corsika_photon, id, &prng),
        EventIoPhotonFactory::BadPhotonWeight);
}

TEST_F(EventIoPhotonFactoryTest, mctracer_accepts_photon_weight_equal_0) {
    const unsigned int id = 1337;
    random::Mt19937 prng(random::ZERO_SEED);
    const std::array<float, 8> corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.0, 433};
    EXPECT_NO_THROW(
        EventIoPhotonFactory cpf(corsika_photon, id, &prng));
}

TEST_F(EventIoPhotonFactoryTest, mctracer_accepts_photon_weight_btw_0_and_1) {
    const unsigned int id = 1337;
    random::Mt19937 prng(random::ZERO_SEED);
    const std::array<float, 8>  corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.4455, 433};
    EXPECT_NO_THROW(
        EventIoPhotonFactory cpf(corsika_photon, id, &prng));
}

TEST_F(EventIoPhotonFactoryTest, zero_weight_is_passed_on_zero_from_prng) {
    const std::array<float, 8>  corsika_photon =
    {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.0, 433};
    const unsigned int id = 1337;
    random::FakeConstant prng(0.0);
    EventIoPhotonFactory cpf(corsika_photon, id, &prng);
    EXPECT_TRUE(cpf.passed_atmosphere());
}

TEST_F(EventIoPhotonFactoryTest, relative_arrival_time_on_ground) {
    float arrival_time_on_dround_in_ns = 1.0;
    const std::array<float, 8>  corsika_photon =
    {1.2, 3.4, 0.0, 0.0, arrival_time_on_dround_in_ns, 1e5, 0.4455, 433};
    const unsigned int id = 1337;
    random::FakeConstant prng(0.0);
    EventIoPhotonFactory cpf(corsika_photon, id, &prng);
    EXPECT_EQ(
        arrival_time_on_dround_in_ns*1e-9,
        cpf.relative_arrival_time_on_ground());
}

TEST_F(EventIoPhotonFactoryTest, correct_rel_time_when_intersecting_ground) {
    eventio::Run corsika_file("telescope.dat");
    while (corsika_file.has_still_events_left()) {
        /*
        read in a corsika eventio event. we remember the relative arrival
        times in the eventio file 'relative_arrival_times_in_corsika_file' and
        compare these to the actual arrival times of the mctracer 
        photons on ground.
        */
        eventio::Event event = corsika_file.next_event();

        vector<float> relative_arrival_times_in_corsika_file;

        vector<Photon> photons;

        random::Mt19937 prng(random::ZERO_SEED);

        for (unsigned int id = 0; id < event.photons.size(); id++) {
            EventIoPhotonFactory factory(
                event.photons.at(id),
                id,
                &prng);
            relative_arrival_times_in_corsika_file.push_back(
                factory.relative_arrival_time_on_ground());
            photons.push_back(factory.get_photon());
        }

        // propagate mctracer photons down to ground
        Frame world;
        world.set_name_pos_rot("world", VEC3_ORIGIN, ROT3_UNITY);

        Disc* ground = world.append<Disc>();
        ground->set_name_pos_rot("ground", VEC3_ORIGIN, ROT3_UNITY);
        const Color* ground_color = &COLOR_GRAY;
        const unsigned int ground_sensor_id = 0;
        ground->set_outer_color(ground_color);
        ground->set_inner_color(ground_color);
        ground->set_radius(1e7);

        sensor::Sensor sensor(ground_sensor_id, ground);
        std::vector<sensor::Sensor*> sensor_vec = {&sensor};
        sensor::Sensors sensor_list(sensor_vec);

        world.init_tree_based_on_mother_child_relations();

        // propagation settings
        PropagationConfig settings;
        settings.use_multithread_when_possible = false;

        // photon propagation
        Photons::propagate_photons_in_scenery_with_settings(
            &photons, &world, &settings, &prng);

        // detect photons in sensors
        sensor_list.clear_history();
        sensor_list.assign_photons(&photons);

        double mean_arrival_time = sensor::arrival_time_mean(
            sensor.photon_arrival_history);

        for (
            unsigned int row = 0;
            row < sensor.photon_arrival_history.size();
            row++
        )
            sensor.photon_arrival_history[row].arrival_time -=
                mean_arrival_time;

        double mean_time_of_corsika_photons = std::accumulate(
            relative_arrival_times_in_corsika_file.begin(),
            relative_arrival_times_in_corsika_file.end(),
            0.0)/static_cast<double>(
                relative_arrival_times_in_corsika_file.size());

        for (unsigned int row = 0;
            row < sensor.photon_arrival_history.size();
            row++
        )
            sensor.photon_arrival_history[row].arrival_time +=
                mean_time_of_corsika_photons;

        // for each photon we compare the relative arrival
        // time written in the in the eventio file with the actual arrival time
        // of the mctracer photon which ran down to the ground.
        for (
            unsigned int i = 0;
            i < sensor.photon_arrival_history.size();
            i++
        ) {
            unsigned int id =
                sensor.photon_arrival_history[i].simulation_truth_id;
            EXPECT_NEAR(
                relative_arrival_times_in_corsika_file.at(id),
                sensor.photon_arrival_history[i].arrival_time,
                1e-11);
        }

        // relative_arrival_times_in_corsika_file
    }
}
