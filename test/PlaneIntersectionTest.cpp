
#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "../Vector3D.h"

using namespace std;

class PlaneIntersectionTest : public ::testing::Test {
 protected:

	GlobalSettings setup;
	Vector3D    pos;
	Rotation3D  rot;
	ReflectionProperties  refl; 
	ColourProperties      colo;
	SurfaceProperties surf;
	double x_width = 2.5;
	double y_width = 1.3;
	Plane plane;
	CartesianFrame world;
	PseudoRandomNumberGenerator dice;
	double wavelength = 433e-9;

  PlaneIntersectionTest() {
    // You can do set-up work for each test here.
  }

  virtual ~PlaneIntersectionTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp() {
  	pos.set(0.0,0.0,0.0);
  	rot.set(0.0,0.0,0.0);

	world.set_frame("world",pos,rot);

	refl.SetReflectionCoefficient(1.0);
	colo.set_RGB_0to255(200,128,128);

	surf.set_color(&colo);
	surf.set_reflection(&refl);

	//------------Plane----------------
	plane.set_frame("My_Plane", pos, rot);
	plane.set_inner_surface(&surf);
	plane.set_outer_surface(&surf);
	plane.set_plane_using_x_and_y_width(x_width, y_width);

	//----------declare relationships------------
	world.set_mother_and_child(&plane);

	//---post initialize the world to calculate all bounding spheres---
	world.setup_tree_based_on_mother_child_relations();
  }
};
//------------------------------------------------------------------------------
TEST_F(PlaneIntersectionTest, frontal) {

	Ray ray(Vector3D(0.0, 0.0, -1.0), Vector3D(0.0, 0.0, 1.0));

	Intersection* intersec = ray.get_first_intersection_in(&world);

	ASSERT_TRUE(intersec->does_intersect());
	EXPECT_EQ(&plane, intersec->get_intersecting_object());
	EXPECT_EQ(Vector3D(0.0, 0.0, 0.0), intersec->get_intersection_vector_in_object_system());
	EXPECT_EQ(Vector3D(0.0, 0.0, 1.0), intersec->get_surface_normal_in_object_system());
}
//------------------------------------------------------------------------------
TEST_F(PlaneIntersectionTest, frontal_lateral_offset_alwas_intersection) {

	for(int x_offset=-10; x_offset<10; x_offset++) {
		for(int y_offset=-10; y_offset<10; y_offset++) {
			double x_support = double(x_offset)*0.01;
			double y_support = double(y_offset)*0.01;


			Ray ray(Vector3D(x_support, y_support, -1.0), Vector3D(0.0, 0.0, 1.0));

			Intersection* intersec = ray.get_first_intersection_in(&world);

			ASSERT_TRUE(intersec->does_intersect());
			EXPECT_EQ(&plane, intersec->get_intersecting_object());
			EXPECT_EQ(Vector3D(x_support, y_support, 0.0), intersec->get_intersection_vector_in_object_system());
			EXPECT_EQ(Vector3D(0.0, 0.0, 1.0), intersec->get_surface_normal_in_object_system());
		}
	}
}
//------------------------------------------------------------------------------
TEST_F(PlaneIntersectionTest, close_miss_x) {

	Ray ray(Vector3D(x_width/2.0+0.01, 0.0, -1.0), Vector3D(0.0, 0.0, 1.0));
	Intersection* intersec = ray.get_first_intersection_in(&world);
	EXPECT_FALSE(intersec->does_intersect());
}
//------------------------------------------------------------------------------
TEST_F(PlaneIntersectionTest, close_miss_y) {

	Ray ray(Vector3D(0.0, y_width/2.0+0.01, -1.0), Vector3D(0.0, 0.0, 1.0));
	Intersection* intersec = ray.get_first_intersection_in(&world);
	EXPECT_FALSE(intersec->does_intersect());
}
//------------------------------------------------------------------------------
TEST_F(PlaneIntersectionTest, move_plane_up) {


 	pos.set(0.0,0.0,0.0);
  	rot.set(0.0,0.0,0.0);

	world.set_frame("world",pos,rot);

	refl.SetReflectionCoefficient(1.0);
	colo.set_RGB_0to255(200,128,128);

	//------------sphere----------------
	pos.set(0.0,0.0,1.0);
	plane.set_frame("MySphere", pos, rot);
	plane.set_inner_surface(&surf);
	plane.set_outer_surface(&surf);
	plane.set_plane_using_x_and_y_width(x_width, y_width);

	//----------declare relationships------------
	world.set_mother_and_child(&plane);

	//---post initialize the world to calculate all bounding spheres---
	world.setup_tree_based_on_mother_child_relations();

	Ray ray(Vector3D(0.0, 0.0, -1.0), Vector3D(0.0, 0.0, 1.0));
	Intersection* intersec = ray.get_first_intersection_in(&world);
	EXPECT_TRUE(intersec->does_intersect());
}