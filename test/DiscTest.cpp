#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/Vector3D.h"

using namespace std;

// The fixture for testing class Foo.
class DiscTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(DiscTest, set_Disc) {

  bool error_detected = false;
  try{

    Disc D;
    D.set_radius(42.0);
    EXPECT_EQ(42.0, D.get_radius_of_sphere_enclosing_all_children());
  }catch(...){
    error_detected = true;
  }
  EXPECT_FALSE(error_detected);
}