#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "HexGridXy.h"
#include "SegmetedReflectorGenerator.h"

class HexGridXyTest : public ::testing::Test {
};
//----------------------------------------------------------------------
TEST_F(HexGridXyTest, Cells_are_inside_boundary) {
 
  for(double outer_radius = 1.65; outer_radius<16.5; outer_radius++) {
    HexGridXy my_grid(outer_radius, 0.62);

    std::vector<Vector3D> grid = my_grid.get_grid();

    for(Vector3D cell : grid)
      EXPECT_TRUE(outer_radius > cell.norm2());
  }
}
//----------------------------------------------------------------------
TEST_F(HexGridXyTest, Reflector_buid) {
  SegmetedReflectorGenerator dream_reflector;

  dream_reflector.set_focal_length(5);
  dream_reflector.set_facet_spacing(0.62);
  dream_reflector.set_max_outer_diameter(4.0);
  dream_reflector.set_hybrid_geometry(0.5);

  const CartesianFrame* refl = dream_reflector.get_reflector();

  EXPECT_TRUE(4.0 > refl->get_radius_of_sphere_enclosing_all_children());
  //std::cout << dream_reflector.get_print();
  //GlobalSettings settings;
  //FreeOrbitCamera free(refl, &settings);

}
//----------------------------------------------------------------------