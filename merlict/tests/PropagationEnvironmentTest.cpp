// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/PropagationEnvironment.h"
namespace ml = merlict;


TEST_CASE("PropagationEnvironmentTest: default_instance", "[merlict]") {
    ml::PropagationEnvironment env;
    CHECK(env.root_frame == &ml::VOID_FRAME);
    CHECK(env.config == &ml::DEFAULT_PROPAGATIONCONFIG);
    CHECK(env.prng == &ml::random::VOID_PRNG);
}
