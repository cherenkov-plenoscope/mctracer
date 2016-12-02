#include "gtest/gtest.h"
#include "PhotonSensor/PhotonSensor.h"
using namespace PhotonSensors;
using namespace PhotonSensor;

class SensorStorageTest : public ::testing::Test {};
//------------------------------------------------------------------------------
// DEFAULT CONSTRUCTOR
//------------------------------------------------------------------------------
TEST_F(SensorStorageTest, default_constructor_yields_empty_Sensors) {

	Sensors sens;
	EXPECT_EQ(0u, sens.size());
}
//------------------------------------------------------------------------------
TEST_F(SensorStorageTest, default_constructor_and_access_not_existing_frame) {

	Sensors sens;
	EXPECT_EQ(0u, sens.size());

	// a frame which is not assigned to a sensor
	Frame dog;
	dog.set_name_pos_rot("dog", Vec3::null, Rot3::null);

	// access invalid frame
	EXPECT_THROW(
		sens.at_frame(&dog), 
		PhotonSensors::Sensors::NoSuchFrame
	);
}
//------------------------------------------------------------------------------
// VECTOR OF SENSORS CONSTRUCTOR
//------------------------------------------------------------------------------
TEST_F(SensorStorageTest, empty_vector_of_sensors_must_yield_empty_Sensors) {

	// Collecting
	std::vector<Sensor*> my_sensors;
	EXPECT_EQ(0u, my_sensors.size());

	Sensors sens(my_sensors);
	EXPECT_EQ(0u, sens.size());
}
//------------------------------------------------------------------------------
TEST_F(SensorStorageTest, empty_Sensors_access_invalid_frame) {

	// Creation
	// a frame which is not assigned to a sensor
	Frame dog;
	dog.set_name_pos_rot("dog", Vec3::null, Rot3::null);

	// Collecting
	std::vector<Sensor*> my_sensors;
	EXPECT_EQ(0u, my_sensors.size());

	//Preparing for access
	Sensors sens(my_sensors);
	EXPECT_EQ(0u, sens.size());
	
	// access invalid frame
	EXPECT_THROW(
		sens.at_frame(&dog), 
		PhotonSensors::Sensors::NoSuchFrame
	);
}
//------------------------------------------------------------------------------
TEST_F(SensorStorageTest, take_over_sensors_from_vector) {

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
	std::vector<Sensor*> my_sensors;
	my_sensors.push_back(&on_tree);
	my_sensors.push_back(&on_house);
	my_sensors.push_back(&on_car);
	EXPECT_EQ(3u, my_sensors.size());

	// Taking over the sensors
	Sensors sens(my_sensors);
	EXPECT_EQ(3u, sens.size());
	EXPECT_TRUE(my_sensors.empty());
}
//------------------------------------------------------------------------------
// USAGE
//------------------------------------------------------------------------------
TEST_F(SensorStorageTest, access_sensors_by_frame) {

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
	std::vector<Sensor*> my_sensors;
	my_sensors.push_back(&on_tree);
	my_sensors.push_back(&on_house);
	my_sensors.push_back(&on_car);
	EXPECT_EQ(3u, my_sensors.size());

	//Preparing for access
	Sensors sens(my_sensors);
	EXPECT_EQ(3u, sens.size());
	
	// access
	EXPECT_EQ(&on_tree, sens.at_frame(&tree));
	EXPECT_EQ(&on_house, sens.at_frame(&house));
	EXPECT_EQ(&on_car, sens.at_frame(&car));
}
//------------------------------------------------------------------------------
TEST_F(SensorStorageTest, assert_no_duplicate_frames) {

	// Creation
	// 1st sensor
	Frame tree;
	tree.set_name_pos_rot("tree", Vec3::null, Rot3::null);
	Sensor on_tree(0u, &tree);

	// 2nd sensor points also to the frame of sensor 1 
	Sensor on_tree2(1u, &tree);
	
	// Collecting
	std::vector<Sensor*> my_sensors;
	my_sensors.push_back(&on_tree);
	my_sensors.push_back(&on_tree2);
	EXPECT_EQ(2u, my_sensors.size());

	//Preparing for access
	EXPECT_THROW(
		Sensors sens(my_sensors), 
		PhotonSensors::Sensors::DuplicateFrame
	);
}
//------------------------------------------------------------------------------
TEST_F(SensorStorageTest, access_non_existing_frame) {

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

	// a frame which is not assigned to a sensor
	Frame dog;
	dog.set_name_pos_rot("dog", Vec3::null, Rot3::null);
	
	// Collecting
	std::vector<Sensor*> my_sensors;
	my_sensors.push_back(&on_tree);
	my_sensors.push_back(&on_house);
	my_sensors.push_back(&on_car);
	EXPECT_EQ(3u, my_sensors.size());

	//Preparing for access
	Sensors sens(my_sensors);
	EXPECT_EQ(3u, sens.size());
	
	// access valid frames
	EXPECT_EQ(&on_tree, sens.at_frame(&tree));
	EXPECT_EQ(&on_house, sens.at_frame(&house));
	EXPECT_EQ(&on_car, sens.at_frame(&car));

	// access invalid frame
	EXPECT_THROW(
		sens.at_frame(&dog), 
		PhotonSensors::Sensors::NoSuchFrame
	);
}
//------------------------------------------------------------------------------
TEST_F(SensorStorageTest, empty_sensors_init) {

	std::vector<Sensor*> empty_sensors;

	Sensors sens(empty_sensors);
	EXPECT_EQ(0u, sens.size());
}
//------------------------------------------------------------------------------
TEST_F(SensorStorageTest, empty_sensor_list_find_sensor_init) {

    std::vector<Sensor*> empty_sensors;
    Sensors sens(empty_sensors);

    Frame car;
    car.set_name_pos_rot("car", Vec3::null, Rot3::null);
    
    PhotonSensors::FindSensorByFrame finder(&car, &sens.by_frame);
    EXPECT_FALSE(finder.frame_is_in_sensors());
}