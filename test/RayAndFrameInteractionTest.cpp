#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Ray.h"
#include "Sphere.h"

using namespace std;

class RayAndFrameInteractionTest : public ::testing::Test {
 protected:

  GlobalSettings settings;    

  RayAndFrameInteractionTest() {
    settings.set_max_number_of_reflections(5);
    settings.set_csv_decimal_presicion(9);
  }

  CartesianFrame* spheres_r1m_in_a_row(
    const uint number_of_spheres,
    Vector3D start,
    const double distance_in_between_spheres_in_m
  ) {

    CartesianFrame *spheres_in_a_row_along_x_r1m;
    spheres_in_a_row_along_x_r1m = new CartesianFrame;

    spheres_in_a_row_along_x_r1m->set_frame(
      "ten_in_a_row", 
      Vector3D(0.0,0.0,0.0),
      Rotation3D(0.0,0.0,0.0)
    );

    ColourProperties* color; 
    color = new ColourProperties;
    color->set_RGB_0to255(200,25,25);

    SurfaceProperties* surf;
    surf = new SurfaceProperties;
    surf->set_color(color);

    for(uint i=0; i<number_of_spheres; i++) {
      Sphere* sphere = new Sphere;

      stringstream name; name << "sphere_number_" << i+1;
      sphere->set_frame(
        name.str(),
        start + Vector3D(distance_in_between_spheres_in_m*i,0.0,0.0),
        Rotation3D(0.0,0.0,0.0)
      );

      sphere->set_sphere(1.0);
      sphere->set_inner_surface(surf);
      sphere->set_outer_surface(surf);   
      spheres_in_a_row_along_x_r1m->set_mother_and_child(sphere);
    }

    spheres_in_a_row_along_x_r1m->setup_tree_based_on_mother_child_relations();

    return spheres_in_a_row_along_x_r1m;
  }

  virtual ~RayAndFrameInteractionTest() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};
//------------------------------------------------------------------------------
TEST_F(RayAndFrameInteractionTest, frame_has_bounding_sphere) {
  CartesianFrame* spheres_in_a_row = 
    spheres_r1m_in_a_row(1440, Vector3D(0.0,0.0,0.0),2.5);

    Ray ray(Vector3D(0.0,0.0,0.0),Vector3D(1.0,0.0,0.0));


    for(uint i=0; i<1e3; i++) {

      std::vector<const CartesianFrame*> frames_with_intersection_in_bounding_sphere;
    
      ray.find_intersection_candidates_in_tree_of_frames(
        spheres_in_a_row,
        &frames_with_intersection_in_bounding_sphere
      );

      EXPECT_EQ(1440, frames_with_intersection_in_bounding_sphere.size());
    }
    //FreeOrbitCamera free(spheres_in_a_row, &settings);
}