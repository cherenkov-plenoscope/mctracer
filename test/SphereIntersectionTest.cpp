
#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/Vector3D.h"

using namespace std;

class SphereIntersectionTest : public ::testing::Test {
protected:

	TracerSettings setup;
	Vector3D    pos;
	Rotation3D  rot; 
	Color*      colo;
	double radius;
	Sphere MySphere;
	Frame world;
	Random::Mt19937 dice;
	double wavelength = 433e-9;
	PropagationEnvironment sphere_test_environment;

  SphereIntersectionTest() {
    // You can do set-up work for each test here.
  }

  virtual ~SphereIntersectionTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp() {

  	dice.set_seed(Random::zero_seed);
  	pos.set(0.0,0.0,0.0);
  	rot.set(0.0,0.0,0.0);

	world.set_name_pos_rot("world",pos,rot);

	colo = new Color(200,128,128);

	//------------sphere----------------
	radius = 1.0;
	MySphere.set_name_pos_rot("MySphere", pos, rot);
	MySphere.set_inner_color(colo);
	MySphere.set_outer_color(colo);
	MySphere.set_sphere_radius(radius);

	//----------declare relationships------------
	world.set_mother_and_child(&MySphere);

	//---post initialize the world to calculate all bounding spheres---
	world.init_tree_based_on_mother_child_relations();

	sphere_test_environment.world_geometry = &world;
	sphere_test_environment.propagation_options = &setup;
	sphere_test_environment.random_engine = &dice;
  }
};
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, frontal) {
 
	double x_pos = -5.0;

	Vector3D Support(x_pos,0.0,0.0);
	Vector3D direction(1.0,0.0,0.0);

	Photon P(Support,direction,wavelength);
	P.propagate_in(&sphere_test_environment);

	ASSERT_EQ(2, P.get_number_of_interactions_so_far() ) << "There should be 2 "
	"interaction stored in the history, 1 for creation of the photon and 1 for"
	"the intersection with the sphere";

	EXPECT_EQ(-radius-x_pos, P.get_accumulative_distance() ) << 
	"The photon was shot from 5m away from the spheres center. The sphere has "
	"radius 1m. So there are 5-1=4m to traverse. ";

	EXPECT_EQ(
		Vector3D::unit_x*(-1),
		P.get_intersection_at(1)->get_surface_normal_in_object_system()
	);
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, emmitting_close_above_surface_tangential) {

	Vector3D Support(0.0,0.0,1.0+1e-9);
	Vector3D direction(1.0,0.0,0.0);

	Photon P(Support,direction,wavelength);
	P.propagate_in(&sphere_test_environment);

	ASSERT_EQ(absorption_in_void, P.get_final_interaction_type() );
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, emmitting_close_above_surface_straigtht_away) {

	Vector3D Support(0.0,0.0,1.0+1e-9);
	Vector3D direction(0.0,0.0,1.0);

	Photon P(Support,direction,wavelength);
	P.propagate_in(&sphere_test_environment);

	ASSERT_EQ(absorption_in_void, P.get_final_interaction_type() );
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, tangential_intersection) {

	Vector3D Support(-5.0,0.0,1.0);
	Vector3D direction(1.0,0.0,0.0);

	Photon P(Support,direction,wavelength);
	P.propagate_in(&sphere_test_environment);

	ASSERT_EQ(2, P.get_number_of_interactions_so_far() );

	Vector3D normal = Vector3D::unit_z;

	EXPECT_NEAR(
		0.0,
		normal.distance_to(
			P.get_intersection_at(1)->get_surface_normal_in_object_system()
		),
		1e-12
	);
}
//------------------------------------------------------------------------------
// NEW INTERSECTION METHOD
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, ray_frontal_intersection) {

  Ray ray_with_intersection(Vector3D(0.0,0.0,-2.0), Vector3D(0.0,0.0,1.0));

  const Intersection* intersec = 
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

  const Intersection* empty_intersec = 
    MySphere.calculate_intersection_with(&ray_without_intersection);

  EXPECT_FALSE(empty_intersec->does_intersect());
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, ray_completely_outside_of_sphere) {

  Ray ray_outside(Vector3D(5.0,0.0,0.0), Vector3D(0.0,0.0,1.0));

  const Intersection* intersec = 
    MySphere.calculate_intersection_with(&ray_outside);

  EXPECT_FALSE(intersec->does_intersect());
}
//------------------------------------------------------------------------------
TEST_F(SphereIntersectionTest, ray_starts_inside_sphere) {

  Ray ray_inside(Vector3D::null, Vector3D(0.0,0.0,1.0));

  const Intersection* intersec = 
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

  const Intersection* intersec = 
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