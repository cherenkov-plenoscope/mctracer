// Copyright 2014 Sebastian A. Mueller
#include "merlict/small_ball.h"
#include <math.h>

namespace merlict {

double radius_to_enclose_ball(Vec3 center, Ball ball) {
    Vec3 d = center - ball.center;
    return d.norm() + ball.radius;
}

uint64_t idx_ball_largest_enclosing_radius(
    const std::vector<Ball> &balls,
    const Vec3 &center
) {
    double largest_radius = radius_to_enclose_ball(center, balls[0]);
    uint64_t idx_largest = 0;
    for (uint64_t idx = 1; idx < balls.size(); idx++) {
        double radius = radius_to_enclose_ball(center, balls[idx]);
        if (radius > largest_radius) {
            largest_radius = radius;
            idx_largest = idx;
        }
    }
    return idx_largest;
}

void one_iteration(
    const std::vector<Ball> &balls,
    Ball *approximate,
    uint64_t iteration
) {
    uint64_t b = idx_ball_largest_enclosing_radius(balls, approximate->center);
    Vec3 direction = balls[b].center - approximate->center;
    double d0 = direction.norm();
    direction = direction/d0;
    Vec3 furthest = approximate->center + direction*(d0 + balls[b].radius);
    approximate->center =
        approximate->center + (furthest - approximate->center) *
        (1.0 / (static_cast<double>(iteration) + 1.0));
    b = idx_ball_largest_enclosing_radius(balls, approximate->center);
    approximate->radius = (approximate->center - balls.at(b).center).norm() +
        balls.at(b).radius;}

Ball many_iterations(const std::vector<Ball> &balls) {
    Ball approximate = balls.at(0);
    for (uint64_t i = 0; i < 100; i++)
        one_iteration(balls, &approximate, i+1);
    return approximate;
}

}  // namespace relleums
