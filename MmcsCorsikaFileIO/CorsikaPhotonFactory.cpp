#include "CorsikaPhotonFactory.h"
//------------------------------------------------------------------------------
CorsikaPhotonFactory::CorsikaPhotonFactory(
	const std::vector<float>& _corsika_photon, 
	const uint _id,
	Random::Generator *_prng
) {
	prng = _prng;
	corsika_photon = _corsika_photon;
	id = _id;

	assert_corsika_photon_has_correct_length();
	assert_photon_weight_is_between_zero_and_one();
	check_once_if_passed_atmosphere();
}
//------------------------------------------------------------------------------
void CorsikaPhotonFactory::check_once_if_passed_atmosphere() {
	passed_through_atmosphere_flag = 
		prng->uniform() <= photon_survival_probability();
}
//------------------------------------------------------------------------------
bool CorsikaPhotonFactory::passed_atmosphere()const {
	return passed_through_atmosphere_flag;
}
//------------------------------------------------------------------------------
Photon* CorsikaPhotonFactory::get_photon() {

	Vector3D causal_dir = causal_direction();

	Ray ray_running_upwards_from_ground_to_pos_of_production(
		intersection_with_xy_floor_plane(),
		causal_dir*(-1)
	);

	double ray_parameter_for_production_height = (
		production_height_in_m() - intersection_with_xy_floor_plane().z()
		)/-causal_dir.z();

	Vector3D causal_support = 
		ray_running_upwards_from_ground_to_pos_of_production.
		PositionOnRay(ray_parameter_for_production_height);

	Photon* cherenkov_photon = 
		new Photon(causal_support, causal_dir, wavelength_in_m());	

	cherenkov_photon->set_id(id);

	return cherenkov_photon;
}
//------------------------------------------------------------------------------
Vector3D CorsikaPhotonFactory::causal_direction()const {

	const double z = sqrt(
		1.0 -
		cosine_between_dir_and_world_x_axis()*
		cosine_between_dir_and_world_x_axis() -
		cosine_between_dir_and_world_y_axis()*
		cosine_between_dir_and_world_y_axis()
	);

	Vector3D causal_dir(
		double(cosine_between_dir_and_world_x_axis()),
		double(cosine_between_dir_and_world_y_axis()),
		z
	);	

	return causal_dir*-1.0;
}
//------------------------------------------------------------------------------
Vector3D CorsikaPhotonFactory::intersection_with_xy_floor_plane()const {
	
	return Vector3D(
		x_pos_on_xy_plane_in_m() - x_core_position_in_m, 
		y_pos_on_xy_plane_in_m() - y_core_position_in_m, 
		0.0
	);
}
//------------------------------------------------------------------------------
double CorsikaPhotonFactory::x_pos_on_xy_plane_in_m()const {
	return x_pos_on_world_x_y_plane_in_cm()*1e-2;
}
//------------------------------------------------------------------------------
double CorsikaPhotonFactory::y_pos_on_xy_plane_in_m()const {
	return y_pos_on_world_x_y_plane_in_cm()*1e-2;
}
//------------------------------------------------------------------------------
double CorsikaPhotonFactory::production_height_in_m()const {
	return production_height_in_cm()*1e-2;
}
//------------------------------------------------------------------------------
double CorsikaPhotonFactory::wavelength_in_m()const {
	return fabs(wavelength_in_nm()*1e-9);
}
//------------------------------------------------------------------------------
bool CorsikaPhotonFactory::row_has_only_zeros()const {
	for(float element : corsika_photon)
		if(element != 0.0)
			return false;
	return true;
}
//------------------------------------------------------------------------------
float CorsikaPhotonFactory::x_pos_on_world_x_y_plane_in_cm()const {
	return corsika_photon.at(x_pos_idx);
}
//------------------------------------------------------------------------------
float CorsikaPhotonFactory::y_pos_on_world_x_y_plane_in_cm()const {
	return corsika_photon.at(y_pos_idx);
}
//------------------------------------------------------------------------------
float CorsikaPhotonFactory::cosine_between_dir_and_world_x_axis()const {
	return corsika_photon.at(x_dir_cos_idx);
}
//------------------------------------------------------------------------------
float CorsikaPhotonFactory::cosine_between_dir_and_world_y_axis()const {
	return corsika_photon.at(y_dir_cos_idx);
}
//------------------------------------------------------------------------------
float CorsikaPhotonFactory::wavelength_in_nm()const {
	return corsika_photon.at(wavelength_idx);
}
//------------------------------------------------------------------------------
float CorsikaPhotonFactory::production_time_in_ns()const {
	return corsika_photon.at(time_idx);
}
//------------------------------------------------------------------------------
float CorsikaPhotonFactory::production_height_in_cm()const {
	return corsika_photon.at(z_emission_height_idx);
}
//------------------------------------------------------------------------------
float CorsikaPhotonFactory::photon_survival_probability()const {
	return corsika_photon.at(photon_weight_idx);
}
//------------------------------------------------------------------------------
void CorsikaPhotonFactory::assert_corsika_photon_has_correct_length()const {

	uint input_size = 8;
	if(corsika_photon.size() != input_size) {
		std::stringstream info;
		info << __FILE__ << " " << __LINE__ << "\n";
		info << "Expected input vector size " << input_size << ", but actual ";
		info << "it is " << corsika_photon.size() << "\n";
		throw BadCorsikaPhotonShape(info.str());
	}
}
//------------------------------------------------------------------------------
void CorsikaPhotonFactory::assert_photon_weight_is_between_zero_and_one()const {

	if(photon_survival_probability() < 0.0 || photon_survival_probability() > 1.0) {
		std::stringstream info;
		info << __FILE__ << " " << __LINE__ << "\n";
		info << "Expected photon weight w: 0.0 >= w > 1.0, but actual ";
		info << "it is: " << photon_survival_probability() << "\n";
		throw BadPhotonWeight(info.str());
	}
}
//------------------------------------------------------------------------------