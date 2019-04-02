// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;


TEST_CASE("SensorStorageTest: default_constructor_yields_empty_Sensors", "[merlict]") {
    ml::sensor::Sensors sens;
    CHECK(sens.size() == 0u);
}

TEST_CASE("SensorStorageTest: default_constructor_and_access_not_existing_frame", "[merlict]") {
    ml::sensor::Sensors sens;
    CHECK(sens.size() == 0u);

    // a frame which is not assigned to a sensor
    ml::Frame dog;
    dog.set_name_pos_rot("dog", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    // access invalid frame
    CHECK_THROWS_AS(sens.at_frame(&dog), ml::sensor::Sensors::NoSuchFrame);
}

// VECTOR OF SENSORS CONSTRUCTOR

TEST_CASE("SensorStorageTest: empty_vector_of_sensors_must_yield_empty_Sensors", "[merlict]") {
    // Collecting
    std::vector<ml::sensor::Sensor*> my_sensors;
    CHECK(my_sensors.size() == 0u);

    ml::sensor::Sensors sens(my_sensors);
    CHECK(sens.size() == 0u);
}

TEST_CASE("SensorStorageTest: empty_Sensors_access_invalid_frame", "[merlict]") {
    // Creation
    // a frame which is not assigned to a sensor
    ml::Frame dog;
    dog.set_name_pos_rot("dog", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    // Collecting
    std::vector<ml::sensor::Sensor*> my_sensors;
    CHECK(my_sensors.size() == 0u);

    // Preparing for access
    ml::sensor::Sensors sens(my_sensors);
    CHECK(sens.size() == 0u);

    // access invalid frame
    CHECK_THROWS_AS(sens.at_frame(&dog), ml::sensor::Sensors::NoSuchFrame);
}

TEST_CASE("SensorStorageTest: not_take_over_sensors_from_vector", "[merlict]") {
    // Creation
    ml::Frame tree;
    tree.set_name_pos_rot("tree", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::sensor::Sensor on_tree(0u, &tree);

    ml::Frame house;
    house.set_name_pos_rot("house", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::sensor::Sensor on_house(1u, &house);

    ml::Frame car;
    car.set_name_pos_rot("car", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::sensor::Sensor on_car(2u, &car);

    // Collecting
    std::vector<ml::sensor::Sensor*> my_sensors;
    my_sensors.push_back(&on_tree);
    my_sensors.push_back(&on_house);
    my_sensors.push_back(&on_car);
    CHECK(my_sensors.size() == 3u);

    // Taking over the sensors
    ml::sensor::Sensors sens(my_sensors);
    CHECK(sens.size() == 3u);
    CHECK(my_sensors.size() == 3u);
}

// USAGE

TEST_CASE("SensorStorageTest: access_sensors_by_frame", "[merlict]") {
    // Creation
    ml::Frame tree;
    tree.set_name_pos_rot("tree", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::sensor::Sensor on_tree(0u, &tree);

    ml::Frame house;
    house.set_name_pos_rot("house", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::sensor::Sensor on_house(1u, &house);

    ml::Frame car;
    car.set_name_pos_rot("car", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::sensor::Sensor on_car(2u, &car);

    // Collecting
    std::vector<ml::sensor::Sensor*> my_sensors;
    my_sensors.push_back(&on_tree);
    my_sensors.push_back(&on_house);
    my_sensors.push_back(&on_car);
    CHECK(my_sensors.size() == 3u);

    // Preparing for access
    ml::sensor::Sensors sens(my_sensors);
    CHECK(sens.size() == 3u);

    // access
    CHECK(sens.at_frame(&tree) == &on_tree);
    CHECK(sens.at_frame(&house) == &on_house);
    CHECK(sens.at_frame(&car) == &on_car);
}

TEST_CASE("SensorStorageTest: assert_no_duplicate_frames", "[merlict]") {
    // Creation
    // 1st sensor
    ml::Frame tree;
    tree.set_name_pos_rot("tree", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::sensor::Sensor on_tree(0u, &tree);

    // 2nd sensor points also to the frame of sensor 1
    ml::sensor::Sensor on_tree2(1u, &tree);

    // Collecting
    std::vector<ml::sensor::Sensor*> my_sensors;
    my_sensors.push_back(&on_tree);
    my_sensors.push_back(&on_tree2);
    CHECK(my_sensors.size() == 2u);

    // Preparing for access
    CHECK_THROWS_AS(ml::sensor::Sensors(my_sensors), ml::sensor::Sensors::DuplicateFrame);
}

TEST_CASE("SensorStorageTest: access_non_existing_frame", "[merlict]") {
    // Creation
    ml::Frame tree;
    tree.set_name_pos_rot("tree", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::sensor::Sensor on_tree(0u, &tree);

    ml::Frame house;
    house.set_name_pos_rot("house", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::sensor::Sensor on_house(1u, &house);

    ml::Frame car;
    car.set_name_pos_rot("car", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::sensor::Sensor on_car(2u, &car);

    // a frame which is not assigned to a sensor
    ml::Frame dog;
    dog.set_name_pos_rot("dog", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    // Collecting
    std::vector<ml::sensor::Sensor*> my_sensors;
    my_sensors.push_back(&on_tree);
    my_sensors.push_back(&on_house);
    my_sensors.push_back(&on_car);
    CHECK(my_sensors.size() == 3u);

    // Preparing for access
    ml::sensor::Sensors sens(my_sensors);
    CHECK(sens.size() == 3u);

    // access valid frames
    CHECK(sens.at_frame(&tree) == &on_tree);
    CHECK(sens.at_frame(&house) == &on_house);
    CHECK(sens.at_frame(&car) == &on_car);

    // access invalid frame
    CHECK_THROWS_AS(sens.at_frame(&dog), ml::sensor::Sensors::NoSuchFrame);
}
