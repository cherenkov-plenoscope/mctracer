#include "gtest/gtest.h"
#include "Core/PropagationEnvironment.h"

class PropagationEnvironmentTest : public ::testing::Test {
protected:

    PropagationEnvironmentTest() {}
    virtual ~PropagationEnvironmentTest() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};
//------------------------------------------------------------------------------
TEST_F(PropagationEnvironmentTest, default_instance) {

    PropagationEnvironment env;

    EXPECT_EQ(&Frame::void_frame, env.world_geometry);
    EXPECT_EQ(&PropagationConfig::default_config, env.propagation_options);
    EXPECT_EQ(&Random::void_generator, env.random_engine);
}