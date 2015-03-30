#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "BiConvexLens.h"
#include "Cameras/FreeOrbitCamera.h"

class BiConvexLensTest : public ::testing::Test {
 protected:
  CartesianFrame* test_bench;
  GlobalSettings settings;
  PseudoRandomNumberGenerator dice;
  PropagationEnvironment lens_test_bench_environment;

  //------------------
  BiConvexLensTest() {
    set_up_settings();
    set_up_test_bench();

    lens_test_bench_environment.world_geometry = test_bench;
    lens_test_bench_environment.propagation_options = &settings;
    lens_test_bench_environment.random_engine = &dice;
    lens_test_bench_environment.assert_completeness();
  }
  //------------------
  void set_up_settings() {
    settings.set_max_number_of_reflections(5);   
  }
  //------------------
  void set_up_test_bench() {
    BiConvexLens* lens = get_preassambled_lens();
    Disc* image_sensor = get_preassambled_sensor_disc();
    test_bench = get_test_bench_frame();

    test_bench->set_mother_and_child(lens);
    test_bench->set_mother_and_child(image_sensor);

    test_bench->setup_tree_based_on_mother_child_relations();     
  }
  //------------------  
  BiConvexLens* get_preassambled_lens() {

    BiConvexLens* lens;
    lens = new BiConvexLens;

    lens->set_frame(
      "little_lens",
      Vector3D::null,
      Rotation3D::null
    );

    Color* lens_color;
    lens_color = new Color(255, 128, 128);

    RefractiveIndex* lens_refractive_index;
    lens_refractive_index = new RefractiveIndex(1.3);

    //////////////////////////////////
    lens->set_outer_color(lens_color);
    lens->set_inner_color(lens_color);
    lens->set_inner_refraction(lens_refractive_index);
    lens->set_curvature_radius_and_diameter(0.7, 0.5);
    return lens;
  }
  //------------------
  Disc* get_preassambled_sensor_disc() {

    Disc* image_sensor;
    image_sensor = new Disc;

    image_sensor->set_frame(
      "sensor_disc",
      Vector3D(0.0, 0.0, -2.0), 
      Rotation3D::null
    );

    Color* sensor_color;
    sensor_color = new Color(128, 255, 128);

    image_sensor->set_outer_color(sensor_color);
    image_sensor->set_inner_color(sensor_color);
    image_sensor->set_disc_radius(0.05); 
    return image_sensor;   
  }
  //------------------
  CartesianFrame* get_test_bench_frame() {

    CartesianFrame* test_bench;
    test_bench = new CartesianFrame;

    test_bench->set_frame(
      "BiConvexLens_test_world",
      Vector3D::null,
      Rotation3D::null
    );  
    return test_bench;
  }
  //------------------
  virtual ~BiConvexLensTest() {
  //------------------
  }
};
//----------------------------------------------------------------------
TEST_F(BiConvexLensTest, send_photon_frontal_into_lens) {

  uint total_propagations = 1e4; 
  uint number_of_photons_reaching_sensor_disc = 0;
  for(uint i=0; i<total_propagations; i++) {

    Photon blue_photon(Vector3D(0.0, 0.0, 1.0), Vector3D(0.0, 0.0, -1.0), 433e-9);
    blue_photon.propagate_in(&lens_test_bench_environment);

    if(3.0 == blue_photon.get_accumulative_distance())
      number_of_photons_reaching_sensor_disc++;
  }

  EXPECT_NEAR(
    0.97,
    double(number_of_photons_reaching_sensor_disc)/double(total_propagations),
    5.0e-2
  );
}
//----------------------------------------------------------------------
TEST_F(BiConvexLensTest, send_photons_frontal_into_lens_with_offset) {

  uint total_propagations = 1e4; 
  double number_of_photons_reaching_center_of_sensor_disc = 0.0;

  for(uint i=0; i<total_propagations; i++) {
    double x_support = (i-0.5*total_propagations)/total_propagations*0.5;

    Photon phot(Vector3D(x_support, 0.0, 1.0), Vector3D(0.0, 0.0, -1.0), 433e-9);
    phot.propagate_in(&lens_test_bench_environment);

    double x_on_sensor_disc = phot.get_intersection_at(
      phot.get_number_of_interactions_so_far()-1
      )->get_intersection_vector_in_object_system().x();
      
    if(fabs(x_on_sensor_disc) < 3e-2)
      number_of_photons_reaching_center_of_sensor_disc++;
  }

  EXPECT_NEAR(
    0.97, 
    number_of_photons_reaching_center_of_sensor_disc/double(total_propagations),
    1.0e-2
  );

  /*FreeOrbitCamera free(
    lens_test_bench_environment.world_geometry, 
    lens_test_bench_environment.propagation_options
  );*/
}