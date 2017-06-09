#include "Plenoscope/NightSkyBackground/Light.h"
#include "Core/PhysicalConstants.h"
#include "Tools/Tools.h"
#include "NightSkyBackground.h"

namespace Plenoscope {
namespace NightSkyBackground {
//------------------------------------------------------------------------------
Light::Light(
	const LightFieldSensor::Geometry *_sensor_geometry, 
	const Function::Func1D* _nsb_flux_vs_wavelength
):
	nsb_flux_vs_wavelength(_nsb_flux_vs_wavelength),
	nsb_cdf(_nsb_flux_vs_wavelength),
	sensor_geometry(_sensor_geometry)
{
	max_tilt_vs_optical_axis_to_throw_photons_in = 
		FOV_RADIUS_OVERHEAD*
		sensor_geometry->max_FoV_radius();

	fov_solid_angle = get_solid_angle_for_opening_angle(
		max_tilt_vs_optical_axis_to_throw_photons_in);

	max_principal_aperture_radius_to_trow_photons_in = 
		APERTURE_RADIUS_OVERHEAD*
		sensor_geometry->expected_imaging_system_max_aperture_radius();

	aperture_area = max_principal_aperture_radius_to_trow_photons_in*
	 	max_principal_aperture_radius_to_trow_photons_in*
	 	M_PI;

	overall_nsb_rate =
		nsb_cdf.get_total_integral_of_distribution()*aperture_area*fov_solid_angle;
}
//------------------------------------------------------------------------------
string Light::get_print()const {

	std::stringstream out;
	out << "NightSkyBackground\n";
	out << "  rate................. " << overall_nsb_rate << " Hz\n";
	out << "  FoV solid angle...... " << fov_solid_angle << " sr\n";
	out << "  FoV radius........... " << 
		Rad2Deg(max_tilt_vs_optical_axis_to_throw_photons_in) << " deg\n";
	out << "  aperture area........ " << aperture_area << " m^2\n";
	out << "  aperture radius...... " << 
		max_principal_aperture_radius_to_trow_photons_in << " m\n";
	out << "  wavelength integral.. " << 
		nsb_cdf.get_total_integral_of_distribution() << " 1/(s sr m^2)\n";
	out << "  flux vs wavelength... " << *nsb_flux_vs_wavelength;
	return out.str();
}
//------------------------------------------------------------------------------
double Light::draw_wavelength(Random::Generator* prng)const {
	return nsb_cdf.draw(prng->uniform());
}
//------------------------------------------------------------------------------
double Light::get_overall_rate()const {
	return overall_nsb_rate;
}
//------------------------------------------------------------------------------
}// NightSkyBackground
}// Plenoscope