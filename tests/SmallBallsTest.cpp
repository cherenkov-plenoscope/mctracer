// Copyright 2018 Sebastian A. Mueller
#include "catch.hpp"
#include <math.h>
#include "merlict/small_ball.h"

using namespace merlict;



TEST_CASE("SmallBallTest: ctor_Ball", "[mctracer]") {
    Ball b;
}

TEST_CASE("SmallBallTest: radius_to_enclose_ball", "[mctracer]") {
    Ball a;
    a.center = Vec3(0,0,0);
    a.radius = 1.0;
    CHECK(1.0 == radius_to_enclose_ball(Vec3(0,0,0), a));

    a.center = Vec3(0,0,1);
    a.radius = 1.0;
    CHECK(2.0 == radius_to_enclose_ball(Vec3(0,0,0), a));

    a.center = Vec3(0,0,1);
    a.radius = 2.0;
    CHECK(3.0 == radius_to_enclose_ball(Vec3(0,0,0), a));
}

TEST_CASE("SmallBallTest: idx_ball_largest_enclosing_radius", "[mctracer]") {
    Ball a;
    a.center = Vec3(0,0,0);
    a.radius = 1.0;

    Ball b;
    b.center = Vec3(0,0,1);
    b.radius = 1.0;

    Ball c;
    c.center = Vec3(0,0,0);
    c.radius = 0.5;

    std::vector<Ball> balls;
    balls.push_back(a);
    balls.push_back(b);
    balls.push_back(c);

    CHECK(1u == idx_ball_largest_enclosing_radius(balls, Vec3(0,0,0)));
}

TEST_CASE("SmallBallTest: balls", "[mctracer]") {
    Ball a;
    a.center = Vec3(0,0,0);
    a.radius = 1.0;

    Ball b;
    b.center = Vec3(0,0,1);
    b.radius = 1.0;

    std::vector<Ball> balls;
    balls.push_back(a);
    balls.push_back(b);

    Ball c = many_iterations(balls);
    CHECK(c.center.x == Approx(0).margin(1e-2));
    CHECK(c.center.y == Approx(0).margin(1e-2));
    CHECK(c.center.z == Approx(0.5).margin(1e-2));
    CHECK(c.radius == Approx(1.5).margin(1e-2));
}

TEST_CASE("SmallBallTest: balls2", "[mctracer]") {
    Ball a;
    a.center = Vec3(0,0,0);
    a.radius = 1.0;

    Ball b;
    b.center = Vec3(1,0,0);
    b.radius = 1.0;

    Ball c;
    c.center = Vec3(1,1,0);
    c.radius = 1.0;

    Ball d;
    d.center = Vec3(0,1,0);
    d.radius = 1.0;

    std::vector<Ball> balls;
    balls.push_back(a);
    balls.push_back(b);
    balls.push_back(c);
    balls.push_back(d);

    Ball r = many_iterations(balls);
    CHECK(r.center.x == Approx(0.5).margin(1e-2));
    CHECK(r.center.y == Approx(0.5).margin(1e-2));
    CHECK(r.center.z == Approx(0.0).margin(1e-2));
    CHECK(r.radius == Approx(sqrt(.5) + 1).margin(1e-2));
}
