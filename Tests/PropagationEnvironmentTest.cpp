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

    EXPECT_EQ(&Frame::VOID_FRAME, env.world_geometry);
    EXPECT_EQ(&PropagationConfig::DEFAULT_CONFIG, env.propagation_options);
    EXPECT_EQ(&Random::void_generator, env.random_engine);
}