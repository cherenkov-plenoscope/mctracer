#include "LightFieldTelescope.h"
#include "Core/PhysicalConstants.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
NightSkyBackgroundLight::NightSkyBackgroundLight(
	const Geometry *_telescope_geometry, 
	const Function::Func1D* _nsb_flux_vs_wavelength
):nsb_cdf(_nsb_flux_vs_wavelength) {

	telescope_geometry = _telescope_geometry;
	nsb_flux_vs_wavelength = _nsb_flux_vs_wavelength;

	max_tilt_vs_optical_axis_to_throw_photons_in = 
		telescope_geometry->max_FoV_radius();

	solid_angle = telescope_geometry->field_of_view_solid_angle();

	max_principal_aperture_radius_to_trow_photons_in = 
		telescope_geometry->principal_aperture_radius_to_throw_photons_in();

	area = max_principal_aperture_radius_to_trow_photons_in*
	 	max_principal_aperture_radius_to_trow_photons_in*
	 	M_PI;

	overall_nsb_rate =
		nsb_cdf.get_total_integral_of_distribution()*area*solid_angle;
}	
//------------------------------------------------------------------------------
std::vector<Photon*>* NightSkyBackgroundLight::get_photons_in_duration(
	const double delay,
	const double duration,
	Random::Generator* prng
) {

	init_relative_arrival_times(duration, prng);

	std::vector<Photon*>* photons = new std::vector<Photon*>;
	photons->reserve(relative_arrival_times.size());

	for(double relative_time: relative_arrival_times)
		photons->push_back(
			get_photon_on_principal_aperture(delay + relative_time,	prng)
		);

	return photons;
}
//------------------------------------------------------------------------------
void NightSkyBackgroundLight::init_relative_arrival_times(
	const double duration,
	Random::Generator* prng
) {

	relative_arrival_times.clear();
	double relative_arrival_times_sum = 0.0;

	while(relative_arrival_times_sum < duration) {

		double time_until_next_photon = prng->expovariate(overall_nsb_rate);

		relative_arrival_times.push_back(relative_arrival_times_sum);
		relative_arrival_times_sum += time_until_next_photon;
	};
}
//------------------------------------------------------------------------------
Photon* NightSkyBackgroundLight::get_photon_on_principal_aperture(
	double time_until_reaching_principal_aperture,
	Random::Generator* prng
)const {

	double wavelength = nsb_cdf.draw(prng->uniform());
		
	Vector3D pos_on_principal_aperture = 
		prng->get_point_on_xy_disc_within_radius(
			max_principal_aperture_radius_to_trow_photons_in
		);

	Vector3D incident_direction_on_principal_aperture = 
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

	Vector3D support_of_photon = back_running_ray.PositionOnRay(
		distance_to_travel_before_intersecting_principal_aperture
	);

	Photon* ph = new Photon(
		support_of_photon,
		incident_direction_on_principal_aperture*(-1.0),
		wavelength
	);

	return ph;
}
//------------------------------------------------------------------------------
std::string NightSkyBackgroundLight::get_print()const {

	std::stringstream out;
	out << "NightSkyBackround\n";
	out << "  rate................. " << overall_nsb_rate << " Hz\n";
	out << "  FoV solid angle...... " << solid_angle << " sr\n";
	out << "  FoV radius........... " << 
		Rad2Deg(max_tilt_vs_optical_axis_to_throw_photons_in) << " deg\n";
	out << "  area................. " << area << " m^2\n";
	out << "  area radius.......... " << 
		max_principal_aperture_radius_to_trow_photons_in << " m\n";
	out << "  wavelength integral.. " << 
		nsb_cdf.get_total_integral_of_distribution() << " 1/(s sr m^2)\n";
	out << "  flux vs wavelength... " << *nsb_flux_vs_wavelength;
	return out.str();
}
//------------------------------------------------------------------------------
}