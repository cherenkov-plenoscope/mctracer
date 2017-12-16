// Copyright 2014 Sebastian A. Mueller
#include "Core/Random/SpherePointPicker.h"
#include <math.h>

namespace Random {

ZenithDistancePicker::ZenithDistancePicker(
        const double min_zenith_distance,
        const double max_zenith_distance):
    z_min((cos(min_zenith_distance)+1.0)/2.0),
    z_range((cos(max_zenith_distance)+1.0)/2.0 - z_min)
{}

double ZenithDistancePicker::draw(Random::Generator* prng)const {
    const double z = z_range * prng->uniform() + z_min;
    return acos(2.0*z - 1.0);
}

UniformPicker::UniformPicker(
        const double min_value,
        const double max_value):
    v_min(min_value),
    v_range(max_value - min_value)
{}

double UniformPicker::draw(Random::Generator* prng)const {
    return v_range * prng->uniform() + v_min;
}

Vec3 draw_point_on_sphere(
    Random::Generator* prng,
    const ZenithDistancePicker& zenith,
    const UniformPicker& azimuth
) {
    const double az = azimuth.draw(prng);
    const double zd = zenith.draw(prng);
    const double sin_zd = sin(zd);
    return Vec3(
        sin_zd*cos(az),
        sin_zd*sin(az),
        cos(zd));
}

}  // namespace Random
