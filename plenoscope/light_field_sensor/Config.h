// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_LIGHTFIELDSENSOR_CONFIG_H_
#define PLENOSCOPE_LIGHTFIELDSENSOR_CONFIG_H_

#include <array>
#include <limits>
#include "merlict/merlict.h"

namespace plenoscope {
namespace light_field_sensor {

struct Config {
    relleums::HomTra3 sensor_plane2imaging_system;
    // The imaging system's expected properties as focal length and max
    // outer aperture radius do not neccessarily match the dimensions
    // of the actual imaging system which will be used.
    // For the best performance of the plenoscope, the expected
    // dimensions should match the actual ones of the imaging system,
    // but to study imperfect imaging systems, where the dimensions
    // differ on purpose, the expected dimensions can differ from
    // the actual ones.
    double expected_imaging_system_focal_length;
    double expected_imaging_system_max_aperture_radius;
    double max_FoV_diameter;
    double pixel_FoV_hex_flat2flat;
    double housing_overhead;
    unsigned int number_of_paxel_on_pixel_diagonal;
    const relleums::function::Func1* lens_refraction;
    const relleums::function::Func1* lens_absorbtion;
    const relleums::function::Func1* bin_reflection;
    Config();

    std::array<float, 273> get_sensor_plane2imaging_system_header()const;
};

static const relleums::function::Func1 pmma_refraction(
    {
        {200e-9, 1.49},
        {1200e-9, 1.49}
    });

static const relleums::function::Func1 perfect_transparency(
    {
        {200e-9, std::numeric_limits<double>::infinity()},
        {1200e-9, std::numeric_limits<double>::infinity()}
    });

}  // namespace light_field_sensor
}  // namespace plenoscope

#endif  // PLENOSCOPE_LIGHTFIELDSENSOR_CONFIG_H_
