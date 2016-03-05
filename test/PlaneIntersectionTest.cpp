#include "gtest/gtest.h"
#include "Core/Function/ConstantFunction.h"
#include "Core/TracerSettings.h"
#include "Core/Rotation3D.h"
#include "Core/Frame.h"
#include "Core/Random/Random.h"
#include "Geometry/Plane.h"
#include "Core/Vector3D.h"

using namespace std;

class PlaneIntersectionTest : public ::testing::Test {
 protected:

	TracerSettings setup;
	Vector3D    pos;
	Rotation3D  rot;
	Function::Constant* refl_vs_wavl;
	Color*      colo;
	double x_width = 2.5;
	double y_width = 1.3;
	Plane plane;
	Frame world;
	Random::Mt19937 dice;
	double wavelength = 433e-9;

  PlaneIntersectionTest() {
    // You can do set-up work for each test here.
  }

  virtual ~PlaneIntersectionTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp() {

  	dice.set_seed(Random::zero_seed);
  	
  	pos.set(0.0,0.0,0.0);
  	rot.set(0.0,0.0,0.0);

	world.set_name_pos_rot("world",pos,rot);

	refl_vs_wavl = new Function::Constant(1.0, Function::Limits(200e-9, 1200e-9));
	colo = new Color(200,128,128);;

	//------------Plane----------------
	plane.set_name_pos_rot("My_Plane", pos, rot);
	plane.set_outer_color(colo);
	plane.set_inner_color(colo);
	plane.set_outer_reflection(refl_vs_wavl);
	plane.set_inner_reflection(refl_vs_wavl);
	plane.set_x_y_width(x_width, y_width);

	//----------declare relationships------------
	world.set_mother_and_child(&plane);

	//---post initialize the world to calculate all bounding spheres---
	world.init_tree_based_on_mother_child_relations();
  }
};
//------------------------------------------------------------------------------
TEST_F(PlaneIntersectionTest, frontal) {

	Ray ray(Vector3D(0.0, 0.0, -1.0), Vector3D(0.0, 0.0, 1.0));

	const Intersection* intersec = ray.get_first_intersection_in(&world);

	ASSERT_TRUE(intersec->does_intersect());
	EXPECT_EQ(&plane, intersec->get_object());
	EXPECT_EQ(Vector3D::null, intersec->get_intersection_vector_in_object_system());
	EXPECT_EQ(Vector3D(0.0, 0.0, 1.0), intersec->get_surface_normal_in_object_system());
}
//------------------------------------------------------------------------------
TEST_F(PlaneIntersectionTest, frontal_lateral_offset_alwas_intersection) {

	for(int x_offset=-10; x_offset<10; x_offset++) {
		for(int y_offset=-10; y_offset<10; y_offset++) {
			double x_support = double(x_offset)*0.01;
			double y_support = double(y_offset)*0.01;


			Ray ray(Vector3D(x_support, y_support, -1.0), Vector3D(0.0, 0.0, 1.0));

			const Intersection* intersec = ray.get_first_intersection_in(&world);

			ASSERT_TRUE(intersec->does_intersect());
			EXPECT_EQ(&plane, intersec->get_object());
			EXPECT_EQ(Vector3D(x_support, y_support, 0.0), intersec->get_intersection_vector_in_object_system());
			EXPECT_EQ(Vector3D(0.0, 0.0, 1.0), intersec->get_surface_normal_in_object_system());
		}
	}
}
//------------------------------------------------------------------------------
TEST_F(PlaneIntersectionTest, close_miss_x) {

	Ray ray(Vector3D(x_width/2.0+0.01, 0.0, -1.0), Vector3D(0.0, 0.0, 1.0));
	const Intersection* intersec = ray.get_first_intersection_in(&world);
	EXPECT_FALSE(intersec->does_intersect());
}
//------------------------------------------------------------------------------
TEST_F(PlaneIntersectionTest, close_miss_y) {

	Ray ray(Vector3D(0.0, y_width/2.0+0.01, -1.0), Vector3D(0.0, 0.0, 1.0));
	const Intersection* intersec = ray.get_first_intersection_in(&world);
	EXPECT_FALSE(intersec->does_intersect());
}
//------------------------------------------------------------------------------
TEST_F(PlaneIntersectionTest, move_plane_up) {

 	pos.set(0.0,0.0,0.0);
  	rot.set(0.0,0.0,0.0);

	world.set_name_pos_rot("world",pos,rot);

	colo = new Color(200,128,128);

	//------------sphere----------------
	pos.set(0.0,0.0,1.0);
	plane.set_name_pos_rot("MySphere", pos, rot);
	plane.set_outer_color(colo);
	plane.set_inner_color(colo);
	plane.set_outer_reflection(refl_vs_wavl);
	plane.set_inner_reflection(refl_vs_wavl);
	plane.set_x_y_width(x_width, y_width);

	//----------declare relationships------------
	world.set_mother_and_child(&plane);

	//---post initialize the world to calculate all bounding spheres---
	world.init_tree_based_on_mother_child_relations();

	Ray ray(Vector3D(0.0, 0.0, -1.0), Vector3D(0.0, 0.0, 1.0));
	const Intersection* intersec = ray.get_first_intersection_in(&world);
	EXPECT_TRUE(intersec->does_intersect());
}