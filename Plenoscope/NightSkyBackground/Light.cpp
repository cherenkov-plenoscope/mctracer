#include "Plenoscope/NightSkyBackground/Light.h"
#include "Core/PhysicalConstants.h"
#include "Tools/Tools.h"

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
		sensor_geometry->max_FoV_radius();

	fov_solid_angle = get_solid_angle_for_opening_angle(
		max_tilt_vs_optical_axis_to_throw_photons_in);

	max_principal_aperture_radius_to_trow_photons_in = 1.05*
		sensor_geometry->expected_imaging_system_max_aperture_radius();

	aperture_area = max_principal_aperture_radius_to_trow_photons_in*
	 	max_principal_aperture_radius_to_trow_photons_in*
	 	M_PI;

	overall_nsb_rate =
		nsb_cdf.get_total_integral_of_distribution()*aperture_area*fov_solid_angle;
}	
//------------------------------------------------------------------------------
vector<Photon> Light::get_photons_in_duration(
	const double delay,
	const double duration,
	Random::Generator* prng
) {
	vector<double> relative_arrival_times = draw_relative_arrival_times(
		duration, 
		prng);

	vector<Photon> photons;
	photons.reserve(relative_arrival_times.size());

	for(double relative_time: relative_arrival_times)
		photons.push_back(
			get_photon_on_principal_aperture(delay + relative_time,	prng)
		);

	return photons;
}
//------------------------------------------------------------------------------
vector<double> Light::draw_relative_arrival_times(
	const double duration,
	Random::Generator* prng
) {
	vector<double> relative_arrival_times;
	double relative_arrival_times_sum = 0.0;

	while(relative_arrival_times_sum < duration) {

		double time_until_next_photon = prng->expovariate(overall_nsb_rate);

		relative_arrival_times.push_back(relative_arrival_times_sum);
		relative_arrival_times_sum += time_until_next_photon;
	};

	return relative_arrival_times;
}
//------------------------------------------------------------------------------
Photon Light::get_photon_on_principal_aperture(
	double time_until_reaching_principal_aperture,
	Random::Generator* prng
)const {

	double wavelength = nsb_cdf.draw(prng->uniform());
		
	Vec3 pos_on_principal_aperture = 
		prng->get_point_on_xy_disc_within_radius(
			max_principal_aperture_radius_to_trow_photons_in
		);

	Vec3 incident_direction_on_principal_aperture = 
		prng->get_point_on_unitsphere_within_polar_distance(
			max_tilt_vs_optical_axis_to_throw_photons_in
		);

	Ray back_running_ray(
		pos_on_principal_aperture, 
		incident_direction_on_principal_aperture
	);

	double distance_to_travel_before_intersecting_principal_aperture = 
		time_until_reaching_principal_aperture*
		PhysicalConstants::speed_of_light_in_vacuum;

	Vec3 support_of_photon = back_running_ray.get_pos_at(
		distance_to_travel_before_intersecting_principal_aperture
	);

	return Photon(
		support_of_photon,
		incident_direction_on_principal_aperture*(-1.0),
		wavelength
	);
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