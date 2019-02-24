// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/PropagationEnvironment.h"

using namespace relleums;

class PropagationEnvironmentTest : public ::testing::Test {};

TEST_F(PropagationEnvironmentTest, default_instance) {
    PropagationEnvironment env;
    EXPECT_EQ(&VOID_FRAME, env.root_frame);
    EXPECT_EQ(&DEFAULT_PROPAGATIONCONFIG, env.config);
    EXPECT_EQ(&random::VOID_PRNG, env.prng);
}
