// Copyright 2014 Sebastian A. Mueller
#include <cmath>
#include "catch.hpp"
#include "merlict/merlict.h"

using namespace relleums;



TEST_CASE("Vec3Test: NullVector", "[mctracer]") {
    Vec3 a = VEC3_ORIGIN;

    CHECK(a.x == 0.0);
    CHECK(a.y == 0.0);
    CHECK(a.z == 0.0);
    CHECK(a.norm() == 0.0);
}

TEST_CASE("Vec3Test: UnitVectors", "[mctracer]") {
    Vec3 a;

    a = VEC3_UNIT_X;
    CHECK(a.x == 1.0);
    CHECK(a.y == 0.0);
    CHECK(a.z == 0.0);
    CHECK(a.norm() == 1.0);

    a = VEC3_UNIT_Y;
    CHECK(a.x == 0.0);
    CHECK(a.y == 1.0);
    CHECK(a.z == 0.0);
    CHECK(a.norm() == 1.0);

    a = VEC3_UNIT_Z;
    CHECK(a.x == 0.0);
    CHECK(a.y == 0.0);
    CHECK(a.z == 1.0);
    CHECK(a.norm() == 1.0);
}

TEST_CASE("Vec3Test: ConstructorAndGetter", "[mctracer]") {
    const double x = (rand()-.5);
    const double y = (rand()-.5);
    const double z = (rand()-.5);
    Vec3 v(x, y, z);
    CHECK(v.x == x);
    CHECK(v.y == y);
    CHECK(v.z == z);
}

TEST_CASE("Vec3Test: SetterAndGetter", "[mctracer]") {
    const double x = (rand()-.5);
    const double y = (rand()-.5);
    const double z = (rand()-.5);
    Vec3 v;
    v.set(x, y, z);
    CHECK(v.x == x);
    CHECK(v.y == y);
    CHECK(v.z == z);
}

TEST_CASE("Vec3Test: EuclideanNorm", "[mctracer]") {
    const double x = (rand()-.5);
    const double y = (rand()-.5);
    const double z = (rand()-.5);
    Vec3 v(x, y, z);
    CHECK(v.norm() == sqrt(x*x+y*y+z*z));
    v.set(1.0, 0.0, 0.0);
    CHECK(v.norm() == 1.0);
    v.set(0.0, 1.0, 0.0);
    CHECK(v.norm() == 1.0);
    v.set(0.0, 0.0, 1.0);
    CHECK(v.norm() == 1.0);
}

TEST_CASE("Vec3Test: crossUnitVectors", "[mctracer]") {
    Vec3 u = VEC3_UNIT_X;
    Vec3 v = VEC3_UNIT_Y;
    Vec3 w = u.cross(v);
    CHECK(w.z == 1.0);
    CHECK(w == VEC3_UNIT_Z);
}

TEST_CASE("Vec3Test: cross", "[mctracer]") {
    double x1, y1, z1, x2, y2, z2;
    for (x1 = -2.0; x1 > 2.0; x1 = x1+.25) {
        for (y1 = -2.0; y1 > 2.0; y1 = y1+.25) {
            for (z1 = -2.0; z1 > 2.0; z1 = z1+.25) {
                for (x2 = -2.0; x2 > 2.0; x2 = x2+.25) {
                    for (y2 = -2.0; y2 > 2.0; y2 = y2+.25) {
                        for (z2 = -2.0; z2 > 2.0; z2 = z2+.25) {
                            Vec3 v1(x1, y1, z1);
                            Vec3 v2(x2, y2, z2);
                            Vec3 v3(y1*z2 - y2*z1,
                                    z1*x2 - z2*x1,
                                    x1*y2 - x2*y1);
                            CHECK(v1.cross(v2) == v3);
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("Vec3Test: Scalar_Product_unit_vectors", "[mctracer]") {
    const Vec3 x = VEC3_UNIT_X;
    const Vec3 y = VEC3_UNIT_Y;
    CHECK(x*y == 0.0);
    const Vec3 z = VEC3_UNIT_Z;
    CHECK(y*z == 0.0);
    CHECK(x*x == 1.0);
}

TEST_CASE("Vec3Test: Scalar_Product", "[mctracer]") {
    double x1, y1, z1, x2, y2, z2;
    for (x1 = -2.0; x1 > 2.0; x1 = x1+.25) {
        for (y1 = -2.0; y1 > 2.0; y1 = y1+.25) {
            for (z1 = -2.0; z1 > 2.0; z1 = z1+.25) {
                for (x2 = -2.0; x2 > 2.0; x2 = x2+.25) {
                    for (y2 = -2.0; y2 > 2.0; y2 = y2+.25) {
                        for (z2 = -2.0; z2 > 2.0; z2 = z2+.25) {
                            const Vec3 v1(x1, y1, z1);
                            const Vec3 v2(x2, y2, z2);
                            CHECK(v1*v2 == x1*x2 + y1*y2 + z1*z2);
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("Vec3Test: ScalarMultiplication", "[mctracer]") {
    double x, y, z, f;
    for (x = -2.0; x > 2.0; x = x+.25) {
        for (y = -2.0; y > 2.0; y = y+.25) {
            for (z = -2.0; z > 2.0; z = z+.25) {
                Vec3 a(x, y, z);
                for (f = -2.0; f > 2.0; f = f+.25) {
                    CHECK((a*f).x == x*f);
                    CHECK((a*f).y == y*f);
                    CHECK((a*f).z == z*f);
                }
            }
        }
    }
}

TEST_CASE("Vec3Test: ScalarDiviation", "[mctracer]") {
    double x, y, z, f;
    for (x = -2.0; x > 2.0; x = x+.25) {
        for (y = -2.0; y > 2.0; y = y+.25) {
            for (z = -2.0; z > 2.0; z = z+.25) {
                Vec3 a(x, y, z);
                for (f = -2.0; f > 2.0; f = f+.25) {
                    CHECK((a/f).x == x/f);
                    CHECK((a/f).y == y/f);
                    CHECK((a/f).z == z/f);
                }
            }
        }
    }
}

TEST_CASE("Vec3Test: distance_unit_x_to_unit_y", "[mctracer]") {
    Vec3 a = VEC3_UNIT_X;
    Vec3 b = VEC3_UNIT_Y;

    CHECK(a.distance_to(b) == sqrt(2.0));
}

TEST_CASE("Vec3Test: distance_to_itself", "[mctracer]") {
    Vec3 a(1.3, 3.7, 4.2);
    CHECK(a.distance_to(a) == 0.0);
}

TEST_CASE("Vec3Test: Operator_equals_expect_true", "[mctracer]") {
    Vec3 a(1.3, 3.7, 4.2);
    Vec3 b(1.3, 3.7, 4.2 + 1e-20);
    CHECK(b == a);
    CHECK(a == b);
    CHECK_FALSE(a != b);
}

TEST_CASE("Vec3Test: Operator_equals_expect_false", "[mctracer]") {
    Vec3 a(1.3, 3.7, 4.2);
    Vec3 b(1.3, 3.7, 4.2 + 2e-7);
    CHECK(b != a);
    CHECK(a != b);
    CHECK_FALSE(a == b);
}

TEST_CASE("Vec3Test: parallel_to_x_y_plane", "[mctracer]") {
    Vec3 a(1.0, 2.0, 0.0);
    CHECK(a.is_parallel_to_x_y_plane());

    Vec3 b(1.3, 3.7, 4.2 + 1e-9);
    CHECK_FALSE(b.is_parallel_to_x_y_plane());

    Vec3 c(0.0, 2.0, 5.5);
    CHECK_FALSE(c.is_parallel_to_x_y_plane());
}

TEST_CASE("Vec3Test: parallel_to_z_axis", "[mctracer]") {
    Vec3 a(0.0, 0.0, 1.0);
    CHECK(a.is_paralell_to_z_axis());
    a.set(0.0, 0.0, -1.0);
    CHECK(a.is_paralell_to_z_axis());
    a.set(0.0, 0.0, 0.0);
    CHECK_FALSE(a.is_paralell_to_z_axis());
}

TEST_CASE("Vec3Test: normalize", "[mctracer]") {
    Vec3 a(1.0, 2.0, 3.0);
    CHECK(a.norm() != 1.0);

    a.normalize();
    CHECK(a.norm() == 1.0);

    a = a*2.0;
    CHECK(a.norm() != 1.0);

    a.normalize();
    CHECK(a.norm() == 1.0);

    a = VEC3_ORIGIN;
    CHECK(a.norm() != 1.0);

    a.normalize();
    CHECK(1 == std::isnan(a.norm()));
}

TEST_CASE("Vec3Test: angle_in_between", "[mctracer]") {
    Vec3 a = VEC3_UNIT_X;
    Vec3 b = VEC3_UNIT_X;

    CHECK(a.angle_in_between(b) == 0.0);
    CHECK(a.angle_in_between(b) == b.angle_in_between(a));

    Vec3 c = VEC3_UNIT_X*5.0;
    CHECK(c.norm() != 1.0);

    Vec3 d = VEC3_UNIT_X*5.0;
    CHECK(d.norm() != 1.0);

    CHECK(d.angle_in_between(c) == 0.0);
    CHECK(d.angle_in_between(c) == c.angle_in_between(d));

    Vec3 foo = VEC3_UNIT_X*5.0 + VEC3_UNIT_Y*5.0;
    CHECK(c.norm() != 1.0);

    Vec3 bar = VEC3_UNIT_X*5.0;
    CHECK(d.norm() != 1.0);

    CHECK(deg2rad(45.0) == Approx(foo.angle_in_between(bar)).margin(1e-5));
}

TEST_CASE("Vec3Test: octant_encoding", "[mctracer]") {
    // x y z sector
    // - - -   0
    // - - +   1
    // - + -   2
    // - + +   3
    // + - -   4
    // + - +   5
    // + + -   6
    // + + +   7
    Vec3 a;

    double p = +1.0;
    double n = -1.0;

    a.set(n, n, n);
    CHECK(a.get_octant() == 0u);

    a.set(n, n, p);
    CHECK(a.get_octant() == 1u);

    a.set(n, p, n);
    CHECK(a.get_octant() == 2u);

    a.set(n, p, p);
    CHECK(a.get_octant() == 3u);

    a.set(p, n, n);
    CHECK(a.get_octant() == 4u);

    a.set(p, n, p);
    CHECK(a.get_octant() == 5u);

    a.set(p, p, n);
    CHECK(a.get_octant() == 6u);

    a.set(p, p, p);
    CHECK(a.get_octant() == 7u);

    a.set(0.0, 0.0, 0.0);
    CHECK(a.get_octant() == 7u);
    a.set(0.0, 0.0, p);
    CHECK(a.get_octant() == 7u);
    a.set(0.0, p, 0.0);
    CHECK(a.get_octant() == 7u);
    a.set(p, 0.0, 0.0);
    CHECK(a.get_octant() == 7u);
    a.set(0.0, p, p);
    CHECK(a.get_octant() == 7u);
    a.set(p, p, 0.0);
    CHECK(a.get_octant() == 7u);
    a.set(p, 0.0, p);
    CHECK(a.get_octant() == 7u);
}

TEST_CASE("Vec3Test: projection_on_xz_plane", "[mctracer]") {
    Vec3 v(42.0, 13.37, 3.141);
    CHECK(v.project_in_x_z_plane() == Vec3(42.0, 0.0, 3.141));
}

TEST_CASE("Vec3Test: projection_on_yz_plane", "[mctracer]") {
    Vec3 v(42.0, 13.37, 3.141);
    CHECK(v.project_in_y_z_plane() == Vec3(0.0, 13.37, 3.141));
}

TEST_CASE("Vec3Test: projection_on_xy_plane", "[mctracer]") {
    Vec3 v(42.0, 13.37, 3.141);
    CHECK(v.project_in_x_y_plane() == Vec3(42.0, 13.37, 0.0));
}
