#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "OctTreeCube.h"

using namespace std;

class OctTreeCubeTest : public ::testing::Test {
protected:
	OctTreeCubeTest() {}

	virtual ~OctTreeCubeTest() {}

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	std::vector<CartesianFrame*> get_frames_diff_pos_and_diff_diameter(
		uint num_of_frames
	)const {
		std::vector<CartesianFrame*> children;

		for(uint i=1; i<=num_of_frames; i++) {
			Sphere* child;
			child = new Sphere;


			std::stringstream name; 
			name << "frame_" << i;

			Vector3D pos(i*4.2,i*1.3,i*3.7);
			Rotation3D rot(i*4.2,i*1.3,i*3.7);

			child->set_frame(name.str(), pos, rot);
			child->set_sphere(i);
			children.push_back(child);
		}

		return children;
	}

	std::vector<CartesianFrame*> get_frames_same_pos_and_same_diameter(
		uint num_of_frames
	)const {
		std::vector<CartesianFrame*> children;

		for(uint i=1; i<=num_of_frames; i++) {
			Sphere* child;
			child = new Sphere;


			std::stringstream name; 
			name << "frame_" << i;

			Vector3D pos(0.0,0.0,0.0);
			Rotation3D rot(0.0,0.0,0.0);

			child->set_frame(name.str(), pos, rot);
			child->set_sphere(1.0);
			children.push_back(child);
		}

		return children;
	}

	virtual void TearDown() {}
};
//------------------------------------------------------------------------------
TEST_F(OctTreeCubeTest, constructor) {

	Vector3D center_of_cube(0.0,0.0,0.0);
	double edge_length_of_cube = 1.23;

	OctTreeCube qube(center_of_cube, edge_length_of_cube);

	EXPECT_EQ(center_of_cube, qube.get_center_position());
	EXPECT_EQ(edge_length_of_cube, qube.get_edge_length());

	EXPECT_EQ(0, qube.get_number_of_child_frames());
	EXPECT_EQ(0, qube.get_number_of_child_cubes());
}
//------------------------------------------------------------------------------
TEST_F(OctTreeCubeTest, max_number_of_frames_in_cube) {

	OctTreeCube a_cube(Vector3D(1.2,3.4,5.6),7.8);
	EXPECT_EQ(7, a_cube.get_max_number_of_frames_in_cube());
}
//------------------------------------------------------------------------------
TEST_F(OctTreeCubeTest, fill_in_different_frames) {

	uint number_of_children = 1337;

	OctTreeCube a_cube(Vector3D(1.2,3.4,5.6),7.8);
	a_cube.fill(get_frames_diff_pos_and_diff_diameter(number_of_children));

	EXPECT_EQ(number_of_children, a_cube.get_number_of_child_frames());
	EXPECT_EQ(8, a_cube.get_number_of_child_cubes());
}
//------------------------------------------------------------------------------
TEST_F(OctTreeCubeTest, fill_in_same_frames) {

	uint number_of_children = 1337;

	OctTreeCube a_cube(Vector3D(1.2,3.4,5.6),7.8);
	a_cube.fill(get_frames_same_pos_and_same_diameter(number_of_children));

	EXPECT_EQ(number_of_children, a_cube.get_number_of_child_frames());
	EXPECT_EQ(8, a_cube.get_number_of_child_cubes());
}
//------------------------------------------------------------------------------
