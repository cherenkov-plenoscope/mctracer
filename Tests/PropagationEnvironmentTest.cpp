// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/PropagationEnvironment.h"

class PropagationEnvironmentTest : public ::testing::Test {};

TEST_F(PropagationEnvironmentTest, default_instance) {
    PropagationEnvironment env;
    EXPECT_EQ(&Frame::VOID_FRAME, env.world_geometry);
    EXPECT_EQ(&PropagationConfig::DEFAULT_CONFIG, env.propagation_options);
    EXPECT_EQ(&Random::VOID_PRNG, env.random_engine);
}
