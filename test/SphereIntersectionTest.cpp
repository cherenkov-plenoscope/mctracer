
#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "../Vector3D.h"

using namespace std;

class SphereIntersectionTest : public ::testing::Test {
 protected:

	GlobalSettings setup;
	Vector3D    pos;
	Rotation3D  rot;
	ReflectionProperties  refl; 
	ColourProperties      colo;
	double radius;
	Sphere MySphere;
	CartesianFrame world;
	PseudoRandomNumberGenerator dice;
	double wavelength = 433e-9;

  SphereIntersectionTest() {
    // You can do set-up work for each test here.
  }

  virtual ~SphereIntersectionTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp() {
  	pos.set(0.0,0.0,0.0);
  	rot.set(0.0,0.0,0.0);

	world.set_frame("world",pos,rot);

	refl.SetReflectionCoefficient(1.0);
	colo.set_RGB_0to255(200,128,128);

	//------------sphere----------------
	radius = 1.0;
	MySphere.set_frame("MySphere", pos, rot);
	MySphere.set_surface_properties(&refl, &colo);
	MySphere.set_sphere(radius);

	//----------declare relationships------------
	world.set_mother_and_child(&MySphere);

	//---post initialize the world to calculate all bounding spheres---
	world.setup_tree_based_on_mother_child_relations();
  }
};
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, frontal) {
 
	double x_pos = -5.0;

	Vector3D Support(x_pos,0.0,0.0);
	Vector3D direction(1.0,0.0,0.0);

	ListOfInteractions history;
	Photon P(Support,direction,wavelength);
	P.propagate(&world,&history,&setup,&dice);

	ASSERT_EQ(1, history.Interactions.size() ) << "There should be exactly 1 "
	"interaction stored in the history expect it to intersect with the sphere "
	" only once.";

	EXPECT_EQ(-radius-x_pos, history.get_accumulative_distance() ) << 
	"The photon was shot from 5m away from the spheres center. The sphere has "
	"radius 1m. So there are 5-1=4m to traverse. ";

	Vector3D normal; normal.set_unit_vector_x(); normal = normal*-1.0;
	EXPECT_EQ(
		normal,
		history.Interactions.at(0)->get_surface_normal_in_object_system()
	);
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, emmitting_close_above_surface_tangential) {

	Vector3D Support(0.0,0.0,1.0+1e-9);
	Vector3D direction(1.0,0.0,0.0);

	ListOfInteractions history;
	Photon P(Support,direction,wavelength);
	P.propagate(&world,&history,&setup,&dice);

	ASSERT_EQ(0, history.Interactions.size() );
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, emmitting_close_above_surface_straigtht_away) {

	Vector3D Support(0.0,0.0,1.0+1e-9);
	Vector3D direction(0.0,0.0,1.0);

	ListOfInteractions history;
	Photon P(Support,direction,wavelength);
	P.propagate(&world,&history,&setup,&dice);

	ASSERT_EQ(0, history.Interactions.size() );
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, tangential_intersection) {

	Vector3D Support(-5.0,0.0,1.0);
	Vector3D direction(1.0,0.0,0.0);

	ListOfInteractions history;
	Photon P(Support,direction,wavelength);
	P.propagate(&world,&history,&setup,&dice);

	ASSERT_EQ(1, history.Interactions.size() );

	Vector3D normal; normal.set_unit_vector_z();

	EXPECT_NEAR(
		0.0,
		normal.distance_to(
			history.Interactions.at(0)->get_surface_normal_in_object_system()
		),
		1e-12
	);
}
//------------------------------------------------------------------------------
// NEW INTERSECTION METHOD
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, ray_frontal_intersection) {

  Ray ray_with_intersection(Vector3D(0.0,0.0,-2.0), Vector3D(0.0,0.0,1.0));

  Intersection* intersec = 
  	MySphere.calculate_intersection_with(&ray_with_intersection);

  ASSERT_TRUE(intersec->does_intersect());
  EXPECT_EQ(intersec->get_intersecting_object(), &MySphere);
  EXPECT_EQ(
  	Vector3D(0.0,0.0,-1.0),
  	intersec->get_intersection_vector_in_object_system()
  );
  EXPECT_EQ(
  	Vector3D(0.0,0.0,-1.0),
  	intersec->get_surface_normal_in_object_system()
  );
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, ray_intersection_but_no_causal_intersection) {

  Ray ray_without_intersection(Vector3D(0.0,0.0,+2.0), Vector3D(0.0,0.0,1.0));

  Intersection* empty_intersec = 
    MySphere.calculate_intersection_with(&ray_without_intersection);

  EXPECT_FALSE(empty_intersec->does_intersect());
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, ray_completely_outside_of_sphere) {

  Ray ray_outside(Vector3D(5.0,0.0,0.0), Vector3D(0.0,0.0,1.0));

  Intersection* intersec = 
    MySphere.calculate_intersection_with(&ray_outside);

  EXPECT_FALSE(intersec->does_intersect());
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, ray_starts_inside_sphere) {

  Ray ray_inside(Vector3D(0.0,0.0,0.0), Vector3D(0.0,0.0,1.0));

  Intersection* intersec = 
    MySphere.calculate_intersection_with(&ray_inside);

  ASSERT_TRUE(intersec->does_intersect());
  EXPECT_EQ(intersec->get_intersecting_object(), &MySphere);
  EXPECT_EQ(
  	Vector3D(0.0,0.0,+1.0),
  	intersec->get_intersection_vector_in_object_system()
  );
  EXPECT_EQ(
  	Vector3D(0.0,0.0,+1.0),
  	intersec->get_surface_normal_in_object_system()
  );
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, ray_tangents_sphere) {

  Ray ray_inside(Vector3D(1.0, 0.0, -2.0), Vector3D(0.0, 0.0, 1.0));

  Intersection* intersec = 
    MySphere.calculate_intersection_with(&ray_inside);

  ASSERT_TRUE(intersec->does_intersect());
  EXPECT_EQ(intersec->get_intersecting_object(), &MySphere);
  EXPECT_EQ(
  	Vector3D(1.0, 0.0, 0.0),
  	intersec->get_intersection_vector_in_object_system())
  ;
  EXPECT_EQ(
  	Vector3D(1.0, 0.0, 0.0),
  	intersec->get_surface_normal_in_object_system()
  );
}
//------------------------------------------------------------------------------