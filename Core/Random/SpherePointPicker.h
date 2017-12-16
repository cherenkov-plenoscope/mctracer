// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_RANDOM_SPHEREPOINTPICKER_H_
#define CORE_RANDOM_SPHEREPOINTPICKER_H_

#include "Core/Vec3.h"
#include "Generator.h"

namespace Random {

struct ZenithDistancePicker {
    const double z_min;
    const double z_range;
    ZenithDistancePicker(
        const double min_zenith_distance,
        const double max_zenith_distance);
    double draw(Random::Generator* prng)const;
};

struct UniformPicker {
    const double v_min;
    const double v_range;
    UniformPicker(const double min_value, const double max_value);
    double draw(Random::Generator* prng)const;
};

Vec3 draw_point_on_sphere(
    Random::Generator* prng,
    const ZenithDistancePicker& zenith,
    const UniformPicker& azimuth);

}  // namespace Random

#endif  // CORE_RANDOM_SPHEREPOINTPICKER_H_
