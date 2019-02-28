// Copyright 2016 Sebastian A. Mueller
#include "plenoscope/night_sky_background/Light.h"
#include <sstream>
#include "merlict/PhysicalConstants.h"
#include "merlict/tools.h"
#include "NightSkyBackground.h"
namespace ml = merlict;

namespace plenoscope {
namespace night_sky_background {

Light::Light(
    const light_field_sensor::Geometry *_sensor_geometry,
    const ml::function::Func1* _flux_vs_wavelength
):
flux_vs_wavelength(_flux_vs_wavelength),
wavelength_probability(_flux_vs_wavelength),
sensor_geometry(_sensor_geometry) {
    fov_radius = FOV_RADIUS_OVERHEAD*sensor_geometry->max_FoV_radius();

    fov_solid_angle = ml::get_solid_angle_for_opening_angle(fov_radius);

    aperture_radius = APERTURE_RADIUS_OVERHEAD*
        sensor_geometry->expected_imaging_system_max_aperture_radius();

    aperture_area = M_PI*aperture_radius*aperture_radius;

    rate = wavelength_probability.get_total_integral_of_distribution()*
        aperture_area*
        fov_solid_angle;
}

std::string Light::str()const {
    std::stringstream out;
    out << "NightSkyBackground\n";
    out << "  rate................. " << rate << " Hz\n";
    out << "  FoV solid angle...... " << fov_solid_angle << " sr\n";
    out << "  FoV radius........... " << ml::rad2deg(fov_radius) << " deg\n";
    out << "  aperture area........ " << aperture_area << " m^2\n";
    out << "  aperture radius...... " << aperture_radius << " m\n";
    out << "  wavelength integral.. " <<
        wavelength_probability.get_total_integral_of_distribution() <<
        " 1/(s sr m^2)\n";
    out << "  flux vs wavelength... " << flux_vs_wavelength->str();
    return out.str();
}

}  // namespace night_sky_background
}  // namespace plenoscope
