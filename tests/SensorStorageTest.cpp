// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/merlict.h"

using namespace merlict;
namespace pse = sensor;



TEST_CASE("SensorStorageTest: default_constructor_yields_empty_Sensors", "[mctracer]") {
    pse::Sensors sens;
    CHECK(sens.size() == 0u);
}

TEST_CASE("SensorStorageTest: default_constructor_and_access_not_existing_frame", "[mctracer]") {
    pse::Sensors sens;
    CHECK(sens.size() == 0u);

    // a frame which is not assigned to a sensor
    Frame dog;
    dog.set_name_pos_rot("dog", VEC3_ORIGIN, ROT3_UNITY);

    // access invalid frame
    CHECK_THROWS_AS(sens.at_frame(&dog), pse::Sensors::NoSuchFrame);
}

// VECTOR OF SENSORS CONSTRUCTOR

TEST_CASE("SensorStorageTest: empty_vector_of_sensors_must_yield_empty_Sensors", "[mctracer]") {
    // Collecting
    std::vector<pse::Sensor*> my_sensors;
    CHECK(my_sensors.size() == 0u);

    pse::Sensors sens(my_sensors);
    CHECK(sens.size() == 0u);
}

TEST_CASE("SensorStorageTest: empty_Sensors_access_invalid_frame", "[mctracer]") {
    // Creation
    // a frame which is not assigned to a sensor
    Frame dog;
    dog.set_name_pos_rot("dog", VEC3_ORIGIN, ROT3_UNITY);

    // Collecting
    std::vector<pse::Sensor*> my_sensors;
    CHECK(my_sensors.size() == 0u);

    // Preparing for access
    pse::Sensors sens(my_sensors);
    CHECK(sens.size() == 0u);

    // access invalid frame
    CHECK_THROWS_AS(sens.at_frame(&dog), pse::Sensors::NoSuchFrame);
}

TEST_CASE("SensorStorageTest: not_take_over_sensors_from_vector", "[mctracer]") {
    // Creation
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);
    pse::Sensor on_tree(0u, &tree);

    Frame house;
    house.set_name_pos_rot("house", VEC3_ORIGIN, ROT3_UNITY);
    pse::Sensor on_house(1u, &house);

    Frame car;
    car.set_name_pos_rot("car", VEC3_ORIGIN, ROT3_UNITY);
    pse::Sensor on_car(2u, &car);

    // Collecting
    std::vector<pse::Sensor*> my_sensors;
    my_sensors.push_back(&on_tree);
    my_sensors.push_back(&on_house);
    my_sensors.push_back(&on_car);
    CHECK(my_sensors.size() == 3u);

    // Taking over the sensors
    pse::Sensors sens(my_sensors);
    CHECK(sens.size() == 3u);
    CHECK(my_sensors.size() == 3u);
}

// USAGE

TEST_CASE("SensorStorageTest: access_sensors_by_frame", "[mctracer]") {
    // Creation
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);
    pse::Sensor on_tree(0u, &tree);

    Frame house;
    house.set_name_pos_rot("house", VEC3_ORIGIN, ROT3_UNITY);
    pse::Sensor on_house(1u, &house);

    Frame car;
    car.set_name_pos_rot("car", VEC3_ORIGIN, ROT3_UNITY);
    pse::Sensor on_car(2u, &car);

    // Collecting
    std::vector<pse::Sensor*> my_sensors;
    my_sensors.push_back(&on_tree);
    my_sensors.push_back(&on_house);
    my_sensors.push_back(&on_car);
    CHECK(my_sensors.size() == 3u);

    // Preparing for access
    pse::Sensors sens(my_sensors);
    CHECK(sens.size() == 3u);

    // access
    CHECK(sens.at_frame(&tree) == &on_tree);
    CHECK(sens.at_frame(&house) == &on_house);
    CHECK(sens.at_frame(&car) == &on_car);
}

TEST_CASE("SensorStorageTest: assert_no_duplicate_frames", "[mctracer]") {
    // Creation
    // 1st sensor
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);
    pse::Sensor on_tree(0u, &tree);

    // 2nd sensor points also to the frame of sensor 1
    pse::Sensor on_tree2(1u, &tree);

    // Collecting
    std::vector<pse::Sensor*> my_sensors;
    my_sensors.push_back(&on_tree);
    my_sensors.push_back(&on_tree2);
    CHECK(my_sensors.size() == 2u);

    // Preparing for access
    CHECK_THROWS_AS(pse::Sensors(my_sensors), pse::Sensors::DuplicateFrame);
}

TEST_CASE("SensorStorageTest: access_non_existing_frame", "[mctracer]") {
    // Creation
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);
    pse::Sensor on_tree(0u, &tree);

    Frame house;
    house.set_name_pos_rot("house", VEC3_ORIGIN, ROT3_UNITY);
    pse::Sensor on_house(1u, &house);

    Frame car;
    car.set_name_pos_rot("car", VEC3_ORIGIN, ROT3_UNITY);
    pse::Sensor on_car(2u, &car);

    // a frame which is not assigned to a sensor
    Frame dog;
    dog.set_name_pos_rot("dog", VEC3_ORIGIN, ROT3_UNITY);

    // Collecting
    std::vector<pse::Sensor*> my_sensors;
    my_sensors.push_back(&on_tree);
    my_sensors.push_back(&on_house);
    my_sensors.push_back(&on_car);
    CHECK(my_sensors.size() == 3u);

    // Preparing for access
    pse::Sensors sens(my_sensors);
    CHECK(sens.size() == 3u);

    // access valid frames
    CHECK(sens.at_frame(&tree) == &on_tree);
    CHECK(sens.at_frame(&house) == &on_house);
    CHECK(sens.at_frame(&car) == &on_car);

    // access invalid frame
    CHECK_THROWS_AS(sens.at_frame(&dog), pse::Sensors::NoSuchFrame);
}
