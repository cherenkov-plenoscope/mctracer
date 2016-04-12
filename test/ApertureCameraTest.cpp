#include "gtest/gtest.h"
#include "Cameras/ApertureCamera.h"

using namespace std;

// The fixture for testing class Foo.
class ApertureCameraTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
	
  ApertureCameraTest() {
    // You can do set-up work for each test here.
  }

  virtual ~ApertureCameraTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};
//----------------------------------------------------------------------
TEST_F(ApertureCameraTest, set_up) {
  ApertureCamera Mamiya645("Mamiya645", 640, 480);
  Mamiya645.set_fStop_sesnorWidth_rayPerPixel(2.3, 6e-2, 5);
  Mamiya645.set_FoV_in_rad(Deg2Rad(45.0));

  Vec3 pos(0.0,0.0,-5.0);
  Rotation3D rot(0.0,0.0,0.0);
  Mamiya645.update_position_and_orientation(pos, rot); 
}
//----------------------------------------------------------------------