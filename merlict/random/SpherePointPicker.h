// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_RANDOM_SPHEREPOINTPICKER_H_
#define MERLICT_RANDOM_SPHEREPOINTPICKER_H_

#include "merlict/Vec3.h"
#include "Generator.h"

namespace merlict {
namespace random {

struct ZenithDistancePicker {
    const double z_min;
    const double z_range;
    ZenithDistancePicker(
        const double min_zenith_distance,
        const double max_zenith_distance);
    double draw(random::Generator* prng)const;
};

struct UniformPicker {
    const double v_min;
    const double v_range;
    UniformPicker(const double min_value, const double max_value);
    double draw(random::Generator* prng)const;
};

Vec3 draw_point_on_sphere(
    random::Generator* prng,
    const ZenithDistancePicker& zenith,
    const UniformPicker& azimuth);

}  // namespace random
}  // namespace merlict

#endif  // MERLICT_RANDOM_SPHEREPOINTPICKER_H_
