// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "Core/mctracer.h"
using std::vector;

using namespace relleums;



TEST_CASE("SensorAssignmentTest: empty_sensors_init", "[mctracer]") {
    vector<sensor::Sensor*> empty_sensors;

    sensor::Sensors sens(empty_sensors);
    CHECK(sens.size() == 0u);
}

TEST_CASE("SensorAssignmentTest: empty_sensor_list_find_sensor_init", "[mctracer]") {
    vector<sensor::Sensor*> empty_sensors;
    sensor::Sensors sens(empty_sensors);

    Frame car;
    car.set_name_pos_rot("car", VEC3_ORIGIN, ROT3_UNITY);

    sensor::FindSensorByFrame finder(&car, &sens.by_frame);
    CHECK(!finder.is_absorbed_by_known_sensor);
}

TEST_CASE("SensorAssignmentTest: single_sensor_find", "[mctracer]") {
    Frame car;
    car.set_name_pos_rot("car", VEC3_ORIGIN, ROT3_UNITY);
    sensor::Sensor on_car(0u, &car);

    vector<sensor::Sensor*> raw_sensors;
    raw_sensors.push_back(&on_car);
    sensor::Sensors sensors(raw_sensors);
    CHECK(sensors.size() == 1u);

    Frame duck;
    duck.set_name_pos_rot("duck", VEC3_ORIGIN, ROT3_UNITY);

    sensor::FindSensorByFrame finder1(&duck, &sensors.by_frame);
    CHECK(!finder1.is_absorbed_by_known_sensor);

    sensor::FindSensorByFrame finder2(&car, &sensors.by_frame);
    CHECK(finder2.is_absorbed_by_known_sensor);

    CHECK(&on_car == finder2.final_sensor);
}

TEST_CASE("SensorAssignmentTest: many_sensor_find", "[mctracer]") {
    // Creation
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);
    sensor::Sensor on_tree(0u, &tree);

    Frame house;
    house.set_name_pos_rot("house", VEC3_ORIGIN, ROT3_UNITY);
    sensor::Sensor on_house(1u, &house);

    Frame car;
    car.set_name_pos_rot("car", VEC3_ORIGIN, ROT3_UNITY);
    sensor::Sensor on_car(2u, &car);

    // Collecting
    vector<sensor::Sensor*> raw_sensors;
    raw_sensors.push_back(&on_tree);
    raw_sensors.push_back(&on_house);
    raw_sensors.push_back(&on_car);
    sensor::Sensors sensors(raw_sensors);
    CHECK(sensors.size() == 3u);

    Frame duck;
    duck.set_name_pos_rot("duck", VEC3_ORIGIN, ROT3_UNITY);

    sensor::FindSensorByFrame find_duck(&duck, &sensors.by_frame);
    CHECK(!find_duck.is_absorbed_by_known_sensor);

    sensor::FindSensorByFrame find_car(&car, &sensors.by_frame);
    CHECK(find_car.is_absorbed_by_known_sensor);
    CHECK(&on_car == find_car.final_sensor);

    sensor::FindSensorByFrame find_house(&house, &sensors.by_frame);
    CHECK(find_house.is_absorbed_by_known_sensor);
    CHECK(&on_house == find_house.final_sensor);

    sensor::FindSensorByFrame find_tree(&tree, &sensors.by_frame);
    CHECK(find_tree.is_absorbed_by_known_sensor);
    CHECK(&on_tree == find_tree.final_sensor);
}

TEST_CASE("SensorAssignmentTest: sort_sensors_by_brame", "[mctracer]") {
    unsigned int n = 1000;
    vector<sensor::Sensor> sensors;
    Frame root;
    root.set_name_pos_rot("root", VEC3_ORIGIN, ROT3_UNITY);
    for (unsigned int i = 0; i < n; i++) {
        Frame* child = root.append<Frame>();
        child->set_name_pos_rot(
            "child"+std::to_string(i),
            VEC3_ORIGIN,
            ROT3_UNITY);
        sensors.push_back(sensor::Sensor(i, child));
    }
    // Collecting
    vector<sensor::Sensor*> raw_sensors;
    for (unsigned int i = 0; i < sensors.size(); i++) {
        raw_sensors.push_back(&sensors.at(i));
    }
    vector<sensor::Sensor*> by_frame = sort_by_frame(&raw_sensors);
    for (unsigned int i = 0; i < by_frame.size()-1; i ++) {
        CHECK(by_frame.at(i)->frame < by_frame.at(i+1)->frame);
    }
}
