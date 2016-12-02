#include "gtest/gtest.h"
#include "PhotonSensor/PhotonSensor.h"
using namespace PhotonSensor;

class SensorAssignmentTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(SensorAssignmentTest, empty_sensors_init) {

	std::vector<Sensor*> empty_sensors;

	Sensors sens(empty_sensors);
	EXPECT_EQ(0u, sens.size());
}
//------------------------------------------------------------------------------
TEST_F(SensorAssignmentTest, empty_sensor_list_find_sensor_init) {

    std::vector<Sensor*> empty_sensors;
    Sensors sens(empty_sensors);

    Frame car;
    car.set_name_pos_rot("car", Vec3::null, Rot3::null);
    
    PhotonSensor::FindSensorByFrame finder(&car, &sens.by_frame);
    EXPECT_FALSE(finder.frame_is_in_sensors());
}