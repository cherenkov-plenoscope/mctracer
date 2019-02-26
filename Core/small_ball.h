// Copyright 2014 Sebastian A. Mueller
//
// Inspired by:
// @article{nielsen2009approximating,
//   title={Approximating smallest enclosing balls with applications to
//   machine learning},
//   author={Nielsen, Frank and Nock, Richard},
//   journal={International Journal of Computational Geometry \& Applications},
//   volume={19},
//   number={05},
//   pages={389--414},
//   year={2009},
//   publisher={World Scientific}
// }

#ifndef CORE_SMALL_BALL_H_
#define CORE_SMALL_BALL_H_

#include <vector>
#include "merlict/Vec3.h"

namespace relleums {

struct Ball {
    Vec3 center;
    double radius;
};

double radius_to_enclose_ball(Vec3 center, Ball ball);

uint64_t idx_ball_largest_enclosing_radius(
    const std::vector<Ball> &balls,
    const Vec3 &center);

void one_iteration(
    const std::vector<Ball> &balls,
    Ball *approximate,
    uint64_t iteration);

Ball many_iterations(const std::vector<Ball> &balls);

}  // namespace relleums

#endif  // CORE_SMALL_BALL_H_
