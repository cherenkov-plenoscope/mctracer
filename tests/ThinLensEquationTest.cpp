// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/geometry/thin_lens.h"
namespace ml = merlict;


TEST_CASE("thin_lensTest: obj_dist_large_vs_focal_dist", "[merlict]") {
    double o = 1.0e9;
    double f = 0.1337;
    CHECK(f == Approx(ml::thin_lens::image_dist_given_focal_and_object_dist(f, o)).margin(1e-9));
}

TEST_CASE("thin_lensTest: fact_set_up_checked_by_hand", "[merlict]") {
    double o = 10.0e3;
    double f = 4.889;
    CHECK(f + 2.391401256e-3 == Approx(ml::thin_lens::image_dist_given_focal_and_object_dist(f, o)).margin(1e-6));
}

TEST_CASE("thin_lensTest: two_f_cross_check", "[merlict]") {
    double f = 4.889;
    double o = 2.0*f;
    double i = 2.0*f;
    CHECK(i == Approx(ml::thin_lens::image_dist_given_focal_and_object_dist(f, o)).margin(1e-9));
    CHECK(o == Approx(ml::thin_lens::object_dist_given_focal_and_image_dist(f, i)).margin(1e-9));
    CHECK(f == Approx(ml::thin_lens::focal_dist_given_object_and_image_dist(o, i)).margin(1e-9));
}
