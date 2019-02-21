// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "PhotonSensor/PhotonSensor.h"
using std::vector;

using namespace relleums;

class SensorAssignmentTest : public ::testing::Test {};

TEST_F(SensorAssignmentTest, empty_sensors_init) {
    vector<PhotonSensor::Sensor*> empty_sensors;

    PhotonSensor::Sensors sens(empty_sensors);
    EXPECT_EQ(0u, sens.size());
}

TEST_F(SensorAssignmentTest, empty_sensor_list_find_sensor_init) {
    vector<PhotonSensor::Sensor*> empty_sensors;
    PhotonSensor::Sensors sens(empty_sensors);

    Frame car;
    car.set_name_pos_rot("car", Vec3::ORIGIN, Rot3::UNITY);

    PhotonSensor::FindSensorByFrame finder(&car, &sens.by_frame);
    EXPECT_FALSE(finder.is_absorbed_by_known_sensor);
}

TEST_F(SensorAssignmentTest, single_sensor_find) {
    Frame car;
    car.set_name_pos_rot("car", Vec3::ORIGIN, Rot3::UNITY);
    PhotonSensor::Sensor on_car(0u, &car);

    vector<PhotonSensor::Sensor*> raw_sensors;
    raw_sensors.push_back(&on_car);
    PhotonSensor::Sensors sensors(raw_sensors);
    EXPECT_EQ(1u, sensors.size());

    Frame duck;
    duck.set_name_pos_rot("duck", Vec3::ORIGIN, Rot3::UNITY);

    PhotonSensor::FindSensorByFrame finder1(&duck, &sensors.by_frame);
    EXPECT_FALSE(finder1.is_absorbed_by_known_sensor);

    PhotonSensor::FindSensorByFrame finder2(&car, &sensors.by_frame);
    EXPECT_TRUE(finder2.is_absorbed_by_known_sensor);

    EXPECT_EQ(finder2.final_sensor, &on_car);
}

TEST_F(SensorAssignmentTest, many_sensor_find) {
    // Creation
    Frame tree;
    tree.set_name_pos_rot("tree", Vec3::ORIGIN, Rot3::UNITY);
    PhotonSensor::Sensor on_tree(0u, &tree);

    Frame house;
    house.set_name_pos_rot("house", Vec3::ORIGIN, Rot3::UNITY);
    PhotonSensor::Sensor on_house(1u, &house);

    Frame car;
    car.set_name_pos_rot("car", Vec3::ORIGIN, Rot3::UNITY);
    PhotonSensor::Sensor on_car(2u, &car);

    // Collecting
    vector<PhotonSensor::Sensor*> raw_sensors;
    raw_sensors.push_back(&on_tree);
    raw_sensors.push_back(&on_house);
    raw_sensors.push_back(&on_car);
    PhotonSensor::Sensors sensors(raw_sensors);
    EXPECT_EQ(3u, sensors.size());

    Frame duck;
    duck.set_name_pos_rot("duck", Vec3::ORIGIN, Rot3::UNITY);

    PhotonSensor::FindSensorByFrame find_duck(&duck, &sensors.by_frame);
    EXPECT_FALSE(find_duck.is_absorbed_by_known_sensor);

    PhotonSensor::FindSensorByFrame find_car(&car, &sensors.by_frame);
    EXPECT_TRUE(find_car.is_absorbed_by_known_sensor);
    EXPECT_EQ(find_car.final_sensor, &on_car);

    PhotonSensor::FindSensorByFrame find_house(&house, &sensors.by_frame);
    EXPECT_TRUE(find_house.is_absorbed_by_known_sensor);
    EXPECT_EQ(find_house.final_sensor, &on_house);

    PhotonSensor::FindSensorByFrame find_tree(&tree, &sensors.by_frame);
    EXPECT_TRUE(find_tree.is_absorbed_by_known_sensor);
    EXPECT_EQ(find_tree.final_sensor, &on_tree);
}

TEST_F(SensorAssignmentTest, sort_sensors_by_brame) {
    unsigned int n = 1000;
    vector<PhotonSensor::Sensor> sensors;
    Frame root;
    root.set_name_pos_rot("root", Vec3::ORIGIN, Rot3::UNITY);
    for (unsigned int i = 0; i < n; i++) {
        Frame* child = root.append<Frame>();
        child->set_name_pos_rot(
            "child"+std::to_string(i),
            Vec3::ORIGIN,
            Rot3::UNITY);
        sensors.push_back(PhotonSensor::Sensor(i, child));
    }
    // Collecting
    vector<PhotonSensor::Sensor*> raw_sensors;
    for (unsigned int i = 0; i < sensors.size(); i++) {
        raw_sensors.push_back(&sensors.at(i));
    }
    vector<PhotonSensor::Sensor*> by_frame = sort_by_frame(&raw_sensors);
    for (unsigned int i = 0; i < by_frame.size()-1; i ++) {
        EXPECT_TRUE(by_frame.at(i)->frame < by_frame.at(i+1)->frame);
    }
}
