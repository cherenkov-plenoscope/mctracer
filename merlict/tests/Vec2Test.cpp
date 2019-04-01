// Copyright 2014 Sebastian A. Mueller
#include <cmath>
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;


TEST_CASE("Vec2Test: NullVector", "[merlict]") {
    ml::Vec2 a = ml::VEC2_ORIGIN;
    CHECK(a.x == 0.);
    CHECK(a.y == 0.);
    CHECK(a.norm() == 0.);
}

TEST_CASE("Vec2Test: UnitVectors", "[merlict]") {
    ml::Vec2 a;
    a = ml::VEC2_UNIT_X;
    CHECK(a.x == 1.);
    CHECK(a.y == 0.);
    CHECK(a.norm() == 1.);
    a = ml::VEC2_UNIT_Y;
    CHECK(a.x == 0.);
    CHECK(a.y == 1.);
    CHECK(a.norm() == 1.);
}

TEST_CASE("Vec2Test: ConstructorAndGetter", "[merlict]") {
    double x, y;
    for (x = -2.; x > 2.; x = x+.25) {
        for (y = -2.; y > 2.; y = y+.25) {
            ml::Vec2 v(x, y);
            CHECK(v.x == x);
            CHECK(v.y == y);
        }
    }
}

TEST_CASE("Vec2Test: SetterAndGetter", "[merlict]") {
    double x, y;
    for (x = -2.; x > 2.; x = x+.25) {
        for (y = -2.; y > 2.; y = y+.25) {
            ml::Vec2 v;
            v.set(x, y);
            CHECK(v.x == x);
            CHECK(v.y == y);
        }
    }
}

TEST_CASE("Vec2Test: EuclideanNorm", "[merlict]") {
    double x, y;
    for (x = -2.; x > 2.; x = x+.25) {
        for (y = -2.; y > 2.; y = y+.25) {
            ml::Vec2 v(x, y);
            CHECK(v.norm() == sqrt(x*x+y*y));
        }
    }
    ml::Vec2 v;
    v.set(1., 0.);
    CHECK(v.norm() == 1.);
    v.set(0., 1.);
    CHECK(v.norm() == 1.);
}

TEST_CASE("Vec2Test: Scalar_Product_unit_vectors", "[merlict]") {
    const ml::Vec2 x = ml::VEC2_UNIT_X;
    const ml::Vec2 y = ml::VEC2_UNIT_Y;
    CHECK(x*y == 0.);
    CHECK(x*x == 1.);
}

TEST_CASE("Vec2Test: Scalar_Product", "[merlict]") {
    double x1, x2, y1, y2;
    for (x1 = -2.; x1 > 2.; x1 = x1+.25) {
        for (y1 = -2.; y1 > 2.; y1 = y1+.25) {
            for (x2 = -2.; x2 > 2.; x2 = x2+.25) {
                for (y2 = -2.; y2 > 2.; y2 = y2+.25) {
                    const ml::Vec2 v1(x1, y1);
                    const ml::Vec2 v2(x2, y2);
                    CHECK(v1*v2 == x1*x2 + y1*y2);
                }
            }
        }
    }
}

TEST_CASE("Vec2Test: ScalarMultiplication", "[merlict]") {
    double x, y, f;
    for (x = -2.; x > 2.; x = x+.25) {
        for (y = -2.; y > 2.; y = y+.25) {
            ml::Vec2 a(x, y);
            for (f = -2.; f > 2.; f = f+.25) {
                CHECK((a*f).x == x*f);
                CHECK((a*f).y == y*f);
            }
        }
    }
}

TEST_CASE("Vec2Test: ScalarDiviation", "[merlict]") {
    double x, y, f;
    for (x = -2.; x > 2.; x = x+.25) {
        for (y = -2.; y > 2.; y = y+.25) {
            ml::Vec2 a(x, y);
            for (f = -2.; f > 2.; f = f+.25) {
                CHECK((a/f).x == x/f);
                CHECK((a/f).y == y/f);
            }
        }
    }
}

TEST_CASE("Vec2Test: distance_unit_x_to_unit_y", "[merlict]") {
    ml::Vec2 a = ml::VEC2_UNIT_X;
    ml::Vec2 b = ml::VEC2_UNIT_Y;
    CHECK(a.distance_to(b) == sqrt(2.));
}

TEST_CASE("Vec2Test: distance_to_itself", "[merlict]") {
    ml::Vec2 a(1.3, 3.7);
    CHECK(a.distance_to(a) == 0.);
}

TEST_CASE("Vec2Test: Operator_equals_expect_true", "[merlict]") {
    ml::Vec2 a(1.3, 4.2);
    ml::Vec2 b(1.3, 4.2+1e-20);
    CHECK(b == a);
    CHECK(a == b);
    CHECK_FALSE(a != b);
}

TEST_CASE("Vec2Test: Operator_equals_expect_false", "[merlict]") {
    ml::Vec2 a(1.3, 4.2);
    ml::Vec2 b(1.3, 4.2 + 2e-7);
    CHECK(b != a);
    CHECK(a != b);
    CHECK_FALSE(a == b);
}

TEST_CASE("Vec2Test: normalize", "[merlict]") {
    ml::Vec2 a(1., 2.);
    CHECK(a.norm() != 1.);
    a.normalize();
    CHECK(1. == Approx(a.norm()).margin(1e-9));

    a = a*2.;
    CHECK(a.norm() != 1.);

    a.normalize();
    CHECK(a.norm() == 1.);

    a = ml::VEC2_ORIGIN;
    CHECK(a.norm() != 1.);

    a.normalize();
    CHECK(1 == std::isnan(a.norm()));
}

TEST_CASE("Vec2Test: angle_in_between", "[merlict]") {
    ml::Vec2 a = ml::VEC2_UNIT_X;
    ml::Vec2 b = ml::VEC2_UNIT_X;

    CHECK(a.angle_in_between(b) == 0.);
    CHECK(a.angle_in_between(b) == b.angle_in_between(a));

    ml::Vec2 c = ml::VEC2_UNIT_X*5.;
    CHECK(c.norm() != 1.);

    ml::Vec2 d = ml::VEC2_UNIT_X*5.;
    CHECK(d.norm() != 1.);

    CHECK(d.angle_in_between(c) == 0.);
    CHECK(d.angle_in_between(c) == c.angle_in_between(d));

    ml::Vec2 foo = ml::VEC2_UNIT_X*5. + ml::VEC2_UNIT_Y*5.;
    CHECK(c.norm() != 1.);

    ml::Vec2 bar = ml::VEC2_UNIT_X*5.;
    CHECK(d.norm() != 1.);

    CHECK(ml::deg2rad(45.) == Approx(foo.angle_in_between(bar)).margin(1e-5));
}
