#include "gtest/gtest.h"
#include "Geometry/TrajectoryFactory.h"

using namespace std;

class TrajectoryFactoryTest : public ::testing::Test {
};
//------------------------------------------------------------------------------
TEST_F(TrajectoryFactoryTest, constructor_using_RayForPropagation) {
  RayForPropagation ray(Vector3D::null, Vector3D(0.0, 0.0, 1.0));

  TrajectoryFactory tra_fac(&ray);

  Frame* tra_frame = tra_fac.get_trajectory();

  EXPECT_EQ(1, tra_frame->get_number_of_children());
}
//------------------------------------------------------------------------------