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
//------------------------------------------------------------------------------
TEST_F(SensorAssignmentTest, single_sensor_find) {
 
	Frame car;
    car.set_name_pos_rot("car", Vec3::null, Rot3::null);
	Sensor on_car(0u, &car); 

	std::vector<Sensor*> raw_sensors;
	raw_sensors.push_back(&on_car);
    Sensors sensors(raw_sensors);
	EXPECT_EQ(1u, sensors.size());

	Frame duck;
	duck.set_name_pos_rot("duck", Vec3::null, Rot3::null);

    PhotonSensor::FindSensorByFrame finder1(&duck, &sensors.by_frame);
    EXPECT_FALSE(finder1.frame_is_in_sensors());

    PhotonSensor::FindSensorByFrame finder2(&car, &sensors.by_frame);
    EXPECT_TRUE(finder2.frame_is_in_sensors());

    EXPECT_EQ(finder2.get_sensor(), &on_car);
}
//------------------------------------------------------------------------------
TEST_F(SensorAssignmentTest, many_sensor_find) {
 
	// Creation
	Frame tree;
	tree.set_name_pos_rot("tree", Vec3::null, Rot3::null);
	Sensor on_tree(0u, &tree);

	Frame house;
	house.set_name_pos_rot("house", Vec3::null, Rot3::null);
	Sensor on_house(1u, &house);

	Frame car;
	car.set_name_pos_rot("car", Vec3::null, Rot3::null);
	Sensor on_car(2u, &car);
	
	// Collecting
	std::vector<Sensor*> raw_sensors;
	raw_sensors.push_back(&on_tree);
	raw_sensors.push_back(&on_house);
	raw_sensors.push_back(&on_car);
	Sensors sensors(raw_sensors);
	EXPECT_EQ(3u, sensors.size());

	Frame duck;
	duck.set_name_pos_rot("duck", Vec3::null, Rot3::null);

    PhotonSensor::FindSensorByFrame find_duck(&duck, &sensors.by_frame);
    EXPECT_FALSE(find_duck.frame_is_in_sensors());

    PhotonSensor::FindSensorByFrame find_car(&car, &sensors.by_frame);
    EXPECT_TRUE(find_car.frame_is_in_sensors());
    EXPECT_EQ(find_car.get_sensor(), &on_car);

    PhotonSensor::FindSensorByFrame find_house(&house, &sensors.by_frame);
    EXPECT_TRUE(find_house.frame_is_in_sensors());
    EXPECT_EQ(find_house.get_sensor(), &on_house);

    PhotonSensor::FindSensorByFrame find_tree(&tree, &sensors.by_frame);
    EXPECT_TRUE(find_tree.frame_is_in_sensors());
    EXPECT_EQ(find_tree.get_sensor(), &on_tree);
}