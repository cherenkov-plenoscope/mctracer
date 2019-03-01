// Copyright 2018 Sebastian A. Mueller
#include "catch.hpp"
#include <math.h>
#include "merlict/small_ball.h"
namespace ml = merlict;


TEST_CASE("SmallBallTest: ctor_Ball", "[merlict]") {
    ml::Ball b;
}

TEST_CASE("SmallBallTest: radius_to_enclose_ball", "[merlict]") {
    ml::Ball a;
    a.center = ml::Vec3(0,0,0);
    a.radius = 1.0;
    CHECK(1.0 == radius_to_enclose_ball(ml::Vec3(0 ,0 ,0), a));

    a.center = ml::Vec3(0,0,1);
    a.radius = 1.0;
    CHECK(2.0 == radius_to_enclose_ball(ml::Vec3(0, 0, 0), a));

    a.center = ml::Vec3(0,0,1);
    a.radius = 2.0;
    CHECK(3.0 == radius_to_enclose_ball(ml::Vec3(0, 0, 0), a));
}

TEST_CASE("SmallBallTest: idx_ball_largest_enclosing_radius", "[merlict]") {
    ml::Ball a;
    a.center = ml::Vec3(0,0,0);
    a.radius = 1.0;

    ml::Ball b;
    b.center = ml::Vec3(0,0,1);
    b.radius = 1.0;

    ml::Ball c;
    c.center = ml::Vec3(0,0,0);
    c.radius = 0.5;

    std::vector<ml::Ball> balls;
    balls.push_back(a);
    balls.push_back(b);
    balls.push_back(c);

    CHECK(1u == ml::idx_ball_largest_enclosing_radius(balls, ml::Vec3(0, 0, 0)));
}

TEST_CASE("SmallBallTest: balls", "[merlict]") {
    ml::Ball a;
    a.center = ml::Vec3(0, 0, 0);
    a.radius = 1.0;

    ml::Ball b;
    b.center = ml::Vec3(0 ,0 ,1);
    b.radius = 1.0;

    std::vector<ml::Ball> balls;
    balls.push_back(a);
    balls.push_back(b);

    ml::Ball c = ml::many_iterations(balls);
    CHECK(c.center.x == Approx(0).margin(1e-2));
    CHECK(c.center.y == Approx(0).margin(1e-2));
    CHECK(c.center.z == Approx(0.5).margin(1e-2));
    CHECK(c.radius == Approx(1.5).margin(1e-2));
}

TEST_CASE("SmallBallTest: balls2", "[merlict]") {
    ml::Ball a;
    a.center = ml::Vec3(0, 0, 0);
    a.radius = 1.0;

    ml::Ball b;
    b.center = ml::Vec3(1, 0, 0);
    b.radius = 1.0;

    ml::Ball c;
    c.center = ml::Vec3(1, 1, 0);
    c.radius = 1.0;

    ml::Ball d;
    d.center = ml::Vec3(0, 1, 0);
    d.radius = 1.0;

    std::vector<ml::Ball> balls;
    balls.push_back(a);
    balls.push_back(b);
    balls.push_back(c);
    balls.push_back(d);

    ml::Ball r = many_iterations(balls);
    CHECK(r.center.x == Approx(0.5).margin(1e-2));
    CHECK(r.center.y == Approx(0.5).margin(1e-2));
    CHECK(r.center.z == Approx(0.0).margin(1e-2));
    CHECK(r.radius == Approx(sqrt(.5) + 1).margin(1e-2));
}
