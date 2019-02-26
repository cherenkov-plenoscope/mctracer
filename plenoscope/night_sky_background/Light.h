// Copyright 2016 Sebastian A. Mueller
#ifndef PLENOSCOPE_NIGHTSKYBACKGROUND_LIGHT_H_
#define PLENOSCOPE_NIGHTSKYBACKGROUND_LIGHT_H_

#include <string>
#include "plenoscope/light_field_sensor/Geometry.h"
#include "merlict/random/random.h"
#include "merlict/function/function.h"

namespace plenoscope {
namespace night_sky_background {

struct Light {
    const merlict::function::Func1* flux_vs_wavelength;
    merlict::random::SamplesFromDistribution wavelength_probability;
    double rate;
    double fov_radius;
    double fov_solid_angle;
    double aperture_radius;
    double aperture_area;
    const light_field_sensor::Geometry *sensor_geometry;
    Light(
        const light_field_sensor::Geometry *_sensor_geometry,
        const merlict::function::Func1* flux_vs_wavelength);
    std::string str()const;
};

}  // namespace night_sky_background
}  // namespace plenoscope

#endif  // PLENOSCOPE_NIGHTSKYBACKGROUND_LIGHT_H_
