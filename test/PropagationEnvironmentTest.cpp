#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/PropagationEnvironment.h"

using namespace std;

class PropagationEnvironmentTest : public ::testing::Test {
 protected:

  PropagationEnvironmentTest() {}
  virtual ~PropagationEnvironmentTest() {}
  virtual void SetUp() {}
  virtual void TearDown() {}
};
//------------------------------------------------------------------------------
TEST_F(PropagationEnvironmentTest, assert_completeness) {

  PropagationEnvironment env;

  Frame world("my_world",Vector3D::null, Rotation3D::null);
  TracerSettings opt;
  PseudoRandomNumberGenerator dice;

  env.world_geometry = &world;
  env.propagation_options = &opt;
  env.random_engine = &dice;

  EXPECT_EQ(&world, env.world_geometry);
  EXPECT_EQ(&opt, env.propagation_options);
  EXPECT_EQ(&dice, env.random_engine);
  EXPECT_NO_THROW(env.assert_completeness());
}
//------------------------------------------------------------------------------
TEST_F(PropagationEnvironmentTest, assert_completeness_on_incomplete_instance) {

  PropagationEnvironment env;

  EXPECT_EQ(nullptr, env.world_geometry);
  EXPECT_EQ(nullptr, env.propagation_options);
  EXPECT_EQ(nullptr, env.random_engine);
  ASSERT_THROW(env.assert_completeness(), TracerException);
}