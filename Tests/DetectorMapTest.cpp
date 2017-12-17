// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Scenery/DetectorMap.h"
using std::string;
using std::vector;

class DetectorMapTest : public ::testing::Test {};

TEST_F(DetectorMapTest, init_empty) {
    DetectorMap my_detectors;
    EXPECT_FALSE(my_detectors.has("red"));
}

TEST_F(DetectorMapTest, get_not_existing_color) {
    DetectorMap my_colors;
    EXPECT_THROW(my_colors.get("red"), DetectorMap::NoSuchKey);
}

TEST_F(DetectorMapTest, add_color_and_check_it_is_in) {
    DetectorMap my_colors;
    vector<PhotonSensor::Sensor*> my_sensors;
    my_colors.add("red", my_sensors);
    EXPECT_TRUE(my_colors.has("red"));
}

TEST_F(DetectorMapTest, add_color_and_add_it_again) {
    DetectorMap my_colors;
    vector<PhotonSensor::Sensor*> my_sensors;
    my_colors.add("red", my_sensors);
    vector<PhotonSensor::Sensor*> my_other_sensors;
    EXPECT_THROW(
        my_colors.add("red", my_other_sensors),
        DetectorMap::KeyAlreadyInUse);
}

TEST_F(DetectorMapTest, add_color_and_get_it_again) {
    DetectorMap my_colors;
    vector<PhotonSensor::Sensor*> novel_red;
    my_colors.add("red", novel_red);
    const vector<PhotonSensor::Sensor*>* red = my_colors.get("red");
    EXPECT_EQ(red->size(), novel_red.size());
}
