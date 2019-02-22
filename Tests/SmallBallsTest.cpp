// Copyright 2018 Sebastian A. Mueller
#include "gtest/gtest.h"
#include <math.h>
#include "Core/small_ball.h"

using namespace relleums;

class SmallBallTest : public ::testing::Test {};

TEST_F(SmallBallTest, ctor_Ball) {
    Ball b;
}

TEST_F(SmallBallTest, radius_to_enclose_ball) {
    Ball a;
    a.center = Vec3(0,0,0);
    a.radius = 1.0;
    EXPECT_EQ(radius_to_enclose_ball(Vec3(0,0,0), a), 1.0);

    a.center = Vec3(0,0,1);
    a.radius = 1.0;
    EXPECT_EQ(radius_to_enclose_ball(Vec3(0,0,0), a), 2.0);

    a.center = Vec3(0,0,1);
    a.radius = 2.0;
    EXPECT_EQ(radius_to_enclose_ball(Vec3(0,0,0), a), 3.0);
}

TEST_F(SmallBallTest, idx_ball_largest_enclosing_radius) {
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

    EXPECT_EQ(idx_ball_largest_enclosing_radius(balls, Vec3(0,0,0)), 1u);
}

TEST_F(SmallBallTest, balls) {
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
    EXPECT_NEAR(c.center.x, 0, 1e-2);
    EXPECT_NEAR(c.center.y, 0, 1e-2);
    EXPECT_NEAR(c.center.z, 0.5, 1e-2);
    EXPECT_NEAR(c.radius, 1.5, 1e-2);
}

TEST_F(SmallBallTest, balls2) {
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
    EXPECT_NEAR(r.center.x, 0.5, 1e-2);
    EXPECT_NEAR(r.center.y, 0.5, 1e-2);
    EXPECT_NEAR(r.center.z, 0.0, 1e-2);
    EXPECT_NEAR(r.radius, sqrt(.5) + 1, 1e-2);
}
