// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/geometry/ThinLensEquation.h"

using namespace merlict;



TEST_CASE("ThinLensEquationTest: obj_dist_large_vs_focal_dist", "[mctracer]") {
    double o = 1.0e9;
    double f = 0.1337;
    CHECK(f == Approx(ThinLensEquation::get_image_dist_given_focal_and_object_dist(f, o)).margin(1e-9));
}

TEST_CASE("ThinLensEquationTest: fact_set_up_checked_by_hand", "[mctracer]") {
    double o = 10.0e3;
    double f = 4.889;
    CHECK(f + 2.391401256e-3 == Approx(ThinLensEquation::get_image_dist_given_focal_and_object_dist(f, o)).margin(1e-6));
}

TEST_CASE("ThinLensEquationTest: two_f_cross_check", "[mctracer]") {
    double f = 4.889;
    double o = 2.0*f;
    double i = 2.0*f;
    CHECK(i == Approx(ThinLensEquation::get_image_dist_given_focal_and_object_dist(f, o)).margin(1e-9));
    CHECK(o == Approx(ThinLensEquation::get_object_dist_given_focal_and_image_dist(f, i)).margin(1e-9));
    CHECK(f == Approx(ThinLensEquation::get_focal_dist_given_object_and_image_dist(o, i)).margin(1e-9));
}
