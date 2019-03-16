// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_FRAMES_H_
#define MERLICT_FRAMES_H_

#include <vector>
#include "merlict/Frame.h"

namespace merlict {
namespace Frames {

double position_spread_in_mother(const std::vector<Frame*> &frames);
Vec3 bounding_sphere_center(const std::vector<Frame*> &frames);
Vec3 bounding_sphere_center_alternative(const std::vector<Frame*> &frames);
Vec3 position_mean_in_mother(const std::vector<Frame*> &frames);
bool positions_in_mother_too_close(
    const std::vector<Frame*> &frames);
double bounding_sphere_radius(
    const std::vector<Frame*> &frames, const Vec3 center);

}  // namespace Frames
}  // namespace merlict

#endif  // MERLICT_FRAMES_H_
