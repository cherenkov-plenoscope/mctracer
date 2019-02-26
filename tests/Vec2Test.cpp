// Copyright 2014 Sebastian A. Mueller
#include <cmath>
#include "catch.hpp"
#include "merlict/merlict.h"

using namespace merlict;



TEST_CASE("Vec2Test: NullVector", "[merlict]") {
    Vec2 a = VEC2_ORIGIN;
    CHECK(a.x == 0.0);
    CHECK(a.y == 0.0);
    CHECK(a.norm() == 0.0);
}

TEST_CASE("Vec2Test: UnitVectors", "[merlict]") {
    Vec2 a;
    a = VEC2_UNIT_X;
    CHECK(a.x == 1.0);
    CHECK(a.y == 0.0);
    CHECK(a.norm() == 1.0);
    a = VEC2_UNIT_Y;
    CHECK(a.x == 0.0);
    CHECK(a.y == 1.0);
    CHECK(a.norm() == 1.0);
}

TEST_CASE("Vec2Test: ConstructorAndGetter", "[merlict]") {
    double x, y;
    for (x = -2.0; x > 2.0; x = x+.25) {
        for (y = -2.0; y > 2.0; y = y+.25) {
            Vec2 v(x, y);
            CHECK(v.x == x);
            CHECK(v.y == y);
        }
    }
}

TEST_CASE("Vec2Test: SetterAndGetter", "[merlict]") {
    double x, y;
    for (x = -2.0; x > 2.0; x = x+.25) {
        for (y = -2.0; y > 2.0; y = y+.25) {
            Vec2 v;
            v.set(x, y);
            CHECK(v.x == x);
            CHECK(v.y == y);
        }
    }
}

TEST_CASE("Vec2Test: EuclideanNorm", "[merlict]") {
    double x, y;
    for (x = -2.0; x > 2.0; x = x+.25) {
        for (y = -2.0; y > 2.0; y = y+.25) {
            Vec2 v(x, y);
            CHECK(v.norm() == sqrt(x*x+y*y));
        }
    }
    Vec2 v;
    v.set(1.0, 0.0);
    CHECK(v.norm() == 1.0);
    v.set(0.0, 1.0);
    CHECK(v.norm() == 1.0);
}

TEST_CASE("Vec2Test: Scalar_Product_unit_vectors", "[merlict]") {
    const Vec2 x = VEC2_UNIT_X;
    const Vec2 y = VEC2_UNIT_Y;
    CHECK(x*y == 0.0);
    CHECK(x*x == 1.0);
}

TEST_CASE("Vec2Test: Scalar_Product", "[merlict]") {
    double x1, x2, y1, y2;
    for (x1 = -2.0; x1 > 2.0; x1 = x1+.25) {
        for (y1 = -2.0; y1 > 2.0; y1 = y1+.25) {
            for (x2 = -2.0; x2 > 2.0; x2 = x2+.25) {
                for (y2 = -2.0; y2 > 2.0; y2 = y2+.25) {
                    const Vec2 v1(x1, y1);
                    const Vec2 v2(x2, y2);
                    CHECK(v1*v2 == x1*x2 + y1*y2);
                }
            }
        }
    }
}

TEST_CASE("Vec2Test: ScalarMultiplication", "[merlict]") {
    double x, y, f;
    for (x = -2.0; x > 2.0; x = x+.25) {
        for (y = -2.0; y > 2.0; y = y+.25) {
            Vec2 a(x, y);
            for (f = -2.0; f > 2.0; f = f+.25) {
                CHECK((a*f).x == x*f);
                CHECK((a*f).y == y*f);
            }
        }
    }
}

TEST_CASE("Vec2Test: ScalarDiviation", "[merlict]") {
    double x, y, f;
    for (x = -2.0; x > 2.0; x = x+.25) {
        for (y = -2.0; y > 2.0; y = y+.25) {
            Vec2 a(x, y);
            for (f = -2.0; f > 2.0; f = f+.25) {
                CHECK((a/f).x == x/f);
                CHECK((a/f).y == y/f);
            }
        }
    }
}

TEST_CASE("Vec2Test: distance_unit_x_to_unit_y", "[merlict]") {
    Vec2 a = VEC2_UNIT_X;
    Vec2 b = VEC2_UNIT_Y;
    CHECK(a.distance_to(b) == sqrt(2.0));
}

TEST_CASE("Vec2Test: distance_to_itself", "[merlict]") {
    Vec2 a(1.3, 3.7);
    CHECK(a.distance_to(a) == 0.0);
}

TEST_CASE("Vec2Test: Operator_equals_expect_true", "[merlict]") {
    Vec2 a(1.3, 4.2);
    Vec2 b(1.3, 4.2+1e-20);
    CHECK(b == a);
    CHECK(a == b);
    CHECK_FALSE(a != b);
}

TEST_CASE("Vec2Test: Operator_equals_expect_false", "[merlict]") {
    Vec2 a(1.3, 4.2);
    Vec2 b(1.3, 4.2 + 2e-7);
    CHECK(b != a);
    CHECK(a != b);
    CHECK_FALSE(a == b);
}

TEST_CASE("Vec2Test: normalize", "[merlict]") {
    Vec2 a(1.0, 2.0);
    CHECK(a.norm() != 1.0);
    a.normalize();
    CHECK(1.0 == Approx(a.norm()).margin(1e-9));

    a = a*2.0;
    CHECK(a.norm() != 1.0);

    a.normalize();
    CHECK(a.norm() == 1.0);

    a = VEC2_ORIGIN;
    CHECK(a.norm() != 1.0);

    a.normalize();
    CHECK(1 == std::isnan(a.norm()));
}

TEST_CASE("Vec2Test: angle_in_between", "[merlict]") {
    Vec2 a = VEC2_UNIT_X;
    Vec2 b = VEC2_UNIT_X;

    CHECK(a.angle_in_between(b) == 0.0);
    CHECK(a.angle_in_between(b) == b.angle_in_between(a));

    Vec2 c = VEC2_UNIT_X*5.0;
    CHECK(c.norm() != 1.0);

    Vec2 d = VEC2_UNIT_X*5.0;
    CHECK(d.norm() != 1.0);

    CHECK(d.angle_in_between(c) == 0.0);
    CHECK(d.angle_in_between(c) == c.angle_in_between(d));

    Vec2 foo = VEC2_UNIT_X*5.0 + VEC2_UNIT_Y*5.0;
    CHECK(c.norm() != 1.0);

    Vec2 bar = VEC2_UNIT_X*5.0;
    CHECK(d.norm() != 1.0);

    CHECK(deg2rad(45.0) == Approx(foo.angle_in_between(bar)).margin(1e-5));
}

TEST_CASE("Vec2Test: quadrant_encoding", "[merlict]") {
    // x y sector
    // - -   0
    // - +   1
    // + -   2
    // + +   3
    Vec2 a;

    double p = +1.0;
    double n = -1.0;

    a.set(n, n);
    CHECK(a.get_quadrant() == 0u);

    a.set(n, p);
    CHECK(a.get_quadrant() == 1u);

    a.set(p, n);
    CHECK(a.get_quadrant() == 2u);

    a.set(p, p);
    CHECK(a.get_quadrant() == 3u);

    a.set(0.0, 0.0);
    CHECK(a.get_quadrant() == 0u);
    a.set(0.0, p);
    CHECK(a.get_quadrant() == 1u);
    a.set(p, 0.0);
    CHECK(a.get_quadrant() == 2u);
    a.set(p, p);
    CHECK(a.get_quadrant() == 3u);
}
