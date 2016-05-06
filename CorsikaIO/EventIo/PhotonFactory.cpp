#include "PhotonFactory.h"
#include "Core/PhysicalConstants.h"
#include "Core/PhotonMcTruth.h"
namespace EventIo {
//------------------------------------------------------------------------------
PhotonFactory::PhotonFactory(
	const array<float, 8> &_corsika_photon, 
	const uint _id,
	Random::Generator *prng
) {
	corsika_photon = _corsika_photon;
	id = _id;

	//assert_corsika_photon_has_correct_length();
	assert_photon_weight_is_between_zero_and_one();
	check_once_if_passed_atmosphere(prng);
}
//------------------------------------------------------------------------------
void PhotonFactory::check_once_if_passed_atmosphere(Random::Generator *prng) {
	passed_through_atmosphere_flag = 
		prng->uniform() <= photon_survival_probability();
}
//------------------------------------------------------------------------------
bool PhotonFactory::passed_atmosphere()const {
	return passed_through_atmosphere_flag;
}
//------------------------------------------------------------------------------
Photon* PhotonFactory::get_photon() {

	Vec3 causal_dir = causal_get_direction();

	Ray ray_running_upwards_from_ground_to_pos_of_production(
		intersection_with_xy_floor_plane(),
		causal_dir*(-1)
	);

	Vec3 causal_support = 
		ray_running_upwards_from_ground_to_pos_of_production.
		get_pos_at(ray_parameter_for_production_point());

	PhotonMcTruth* mc_truth = new PhotonMcTruth();
	mc_truth->production_height_over_sea_level = production_height_in_m();

	Photon* cherenkov_photon = 
		new Photon(causal_support, causal_dir, wavelength_in_m(), mc_truth);	

	cherenkov_photon->set_id(id);

	return cherenkov_photon;
}
//------------------------------------------------------------------------------
Vec3 PhotonFactory::causal_get_direction()const {

	const double z = sqrt(
		1.0 -
		corsika_photon[x_dir_cos_idx]*
		corsika_photon[x_dir_cos_idx] -
		corsika_photon[y_dir_cos_idx]*
		corsika_photon[y_dir_cos_idx]
	);

	Vec3 causal_dir(
		corsika_photon[x_dir_cos_idx],
		corsika_photon[y_dir_cos_idx],
		z
	);	

	return causal_dir*-1.0;
}
//------------------------------------------------------------------------------
Vec3 PhotonFactory::intersection_with_xy_floor_plane()const {
	
	return Vec3( 
		x_pos_on_xy_plane_in_m(), 
		y_pos_on_xy_plane_in_m(), 
		0.0
	);
}
//------------------------------------------------------------------------------
double PhotonFactory::production_distance_offset()const {
	// an arbitrary offset distance for the photons to travel until they
	// reach the ground. If set to zero 0.0, the distance for a mctracer photon 
	// to travel is only defined by the relative_arrival_time_on_ground().
	// Ensure this offset is at least as big as your detector system on ground.
	return 1e3;
}
//------------------------------------------------------------------------------
double PhotonFactory::ray_parameter_for_production_point()const {
	return relative_arrival_time_on_ground()*
		PhysicalConstants::speed_of_light_in_vacuum + production_distance_offset();
}
//------------------------------------------------------------------------------
double PhotonFactory::x_pos_on_xy_plane_in_m()const {
	return x_pos_on_world_x_y_plane_in_cm()*1e-2;
}
//------------------------------------------------------------------------------
double PhotonFactory::y_pos_on_xy_plane_in_m()const {
	return y_pos_on_world_x_y_plane_in_cm()*1e-2;
}
//------------------------------------------------------------------------------
double PhotonFactory::production_height_in_m()const {
	return production_height_in_cm()*1e-2;
}
//------------------------------------------------------------------------------
double PhotonFactory::wavelength_in_m()const {
	return fabs(wavelength_in_nm()*1e-9);
}
//------------------------------------------------------------------------------
double PhotonFactory::relative_arrival_time_on_ground()const {
	return corsika_photon[time_idx]*1e-9;
}
//------------------------------------------------------------------------------
float PhotonFactory::x_pos_on_world_x_y_plane_in_cm()const {
	return corsika_photon[x_pos_idx];
}
//------------------------------------------------------------------------------
float PhotonFactory::y_pos_on_world_x_y_plane_in_cm()const {
	return corsika_photon[y_pos_idx];
}
//------------------------------------------------------------------------------
float PhotonFactory::wavelength_in_nm()const {
	return corsika_photon[wavelength_idx];
}
//------------------------------------------------------------------------------
float PhotonFactory::production_height_in_cm()const {
	return corsika_photon[z_emission_height_idx];
}
//------------------------------------------------------------------------------
float PhotonFactory::photon_survival_probability()const {
	return corsika_photon[photon_weight_idx];
}
//------------------------------------------------------------------------------
void PhotonFactory::assert_photon_weight_is_between_zero_and_one()const {

	if(photon_survival_probability() < 0.0 || photon_survival_probability() > 1.0) {
		stringstream info;
		info << __FILE__ << " " << __LINE__ << "\n";
		info << "Expected photon weight w: 0.0 >= w >= 1.0, but actual ";
		info << "it is: " << photon_survival_probability() << "\n";
		throw BadPhotonWeight(info.str());
	}
}
//------------------------------------------------------------------------------
}// EventIo