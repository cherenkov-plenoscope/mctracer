// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "Core/PropagationEnvironment.h"

using namespace relleums;



TEST_CASE("PropagationEnvironmentTest: default_instance", "[mctracer]") {
    PropagationEnvironment env;
    CHECK(env.root_frame == &VOID_FRAME);
    CHECK(env.config == &DEFAULT_PROPAGATIONCONFIG);
    CHECK(env.prng == &random::VOID_PRNG);
}
