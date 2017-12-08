// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORE_FRAMES_H_
#define MCTRACER_CORE_FRAMES_H_

#include <vector>
#include "Core/Frame.h"

namespace Frames {

double spread_of_frame_position_in_mother(const std::vector<Frame*> &frames);
Vec3 bounding_sphere_center(const std::vector<Frame*> &frames);
Vec3 dumb_bounding_sphere_center(const std::vector<Frame*> &frames);
Vec3 mean_of_positions_in_mother(const std::vector<Frame*> &frames);
bool positions_in_mother_are_too_close_together(
    const std::vector<Frame*> &frames);
double bounding_sphere_radius(
    const std::vector<Frame*> &frames, const Vec3 center);

}  // namespace Frames

#endif  // MCTRACER_CORE_FRAMES_H_
