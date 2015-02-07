#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "BiConvexLens.h"
#include "Cameras/FreeOrbitCamera.h"

class BiConvexLensTest : public ::testing::Test {
 protected:
  CartesianFrame* test_bench;
	
  const ColourProperties* lens_color;
  const ColourProperties* sensor_color;
  const RefractiveIndex* lens_refractive_index;

  GlobalSettings settings;    

  BiConvexLensTest() {

    set_up_settings();
    set_up_test_bench();
  }

  void set_up_settings() {

    settings.set_max_number_of_reflections(5);   
  }

  void set_up_test_bench() {

    set_up_colors();
    set_up_lens_refraction_index();

    BiConvexLens* lens = get_preassambled_lens();
    Disc* image_sensor = get_preassambled_sensor_disc();
    test_bench = get_test_bench_frame();

    test_bench->set_mother_and_child(lens);
    test_bench->set_mother_and_child(image_sensor);

    test_bench->setup_tree_based_on_mother_child_relations();     
  }

  void set_up_colors() {

    lens_color = new ColourProperties(255, 128, 128);
    sensor_color = new ColourProperties(128, 255, 128);  
  }

  void set_up_lens_refraction_index() {

    lens_refractive_index = new RefractiveIndex(1.3);
  }

  BiConvexLens* get_preassambled_lens() {

    BiConvexLens* lens;
    lens = new BiConvexLens;

    lens->set_frame(
      "little_lens",
      Vector3D(0.0, 0.0, 0.0),
      Rotation3D(0.0, 0.0, 0.0)
    );

    lens->set_color(lens_color);
    lens->set_refraction_properties(lens_refractive_index);
    lens->set_focal_length_and_diameter(2.0, 0.5);
    return lens;
  }

  Disc* get_preassambled_sensor_disc() {

    Disc* image_sensor;
    image_sensor = new Disc;

    image_sensor->set_frame(
      "sensor_disc",
      Vector3D(0.0, 0.0, -2.0), 
      Rotation3D(0.0, 0.0, 0.0)
    );

    image_sensor->set_color(sensor_color);
    image_sensor->set_disc_radius(0.05); 
    return image_sensor;   
  }

  CartesianFrame* get_test_bench_frame() {

    CartesianFrame* test_bench;
    test_bench = new CartesianFrame;

    test_bench->set_frame(
      "BiConvexLens_test_world",
      Vector3D(0.0, 0.0, 0.0),
      Rotation3D(0.0, 0.0, 0.0)
    );  
    return test_bench;
  }

  virtual ~BiConvexLensTest() {

  }
};
//----------------------------------------------------------------------
TEST_F(BiConvexLensTest, send_photon_frontal_into_lens) {

  PseudoRandomNumberGenerator dice;
 
  Photon blue_photon(Vector3D(0.0, 0.0, 1.0), Vector3D(0.0, 0.0, -1.0), 433e-9);
  
  ListOfInteractions blue_photons_history;

  blue_photon.propagate(
    test_bench,
    &blue_photons_history,
    &settings,
    &dice
  );

  EXPECT_EQ(3.0, blue_photons_history.get_accumulative_distance());

  //std::cout << blue_photons_history << "\n" << test_bench->get_tree_print();
  //FreeOrbitCamera free(test_bench, &settings);
}