#include "LightSourceFromConfig.h"
//------------------------------------------------------------------------------
LightSourceFromConfig::LightSourceFromConfig(const KeyValueMap::Map _config) {
	config = _config;
	std::string type = config.get_value_for_key("type");

	if(StringTools::is_equal(type, "point_like"))
		point_like_source();
	else if(StringTools::is_equal(type, "parallel_disc"))
		parallel_from_disc();
	else
		unknown_type();

	transform_photons();
}
//------------------------------------------------------------------------------
void LightSourceFromConfig::point_like_source() {
	photon_bunch = 
		PhotonBunch::Source::point_like_towards_z_opening_angle_num_photons(
			Deg2Rad(config.get_value_for_key_as_double("opening_angle_in_deg")),
			config.get_value_for_key_as_int("number_of_photons")
		);
}
//------------------------------------------------------------------------------
void LightSourceFromConfig::parallel_from_disc() {
	photon_bunch = 
		PhotonBunch::Source::parallel_towards_z_from_xy_disc(
			config.get_value_for_key_as_double("disc_radius_in_m"),
			config.get_value_for_key_as_int("number_of_photons")
		);
}
//------------------------------------------------------------------------------
void LightSourceFromConfig::unknown_type() {

	std::stringstream info;
	info << "LightSourceFromConfig " << __FILE__ << ", " << __LINE__ << "\n";
	if(!config.get_filename().empty()) info << config.get_filename() << "\n";
	info << "Do not know any light source type called: '";
	info << config.get_value_for_key("type") << "'\n";
	throw TracerException(info.str());
}
//------------------------------------------------------------------------------
void LightSourceFromConfig::transform_photons() {

	const Rotation3D source_rot(
		Deg2Rad(config.get_value_for_key_as_double("source_rot_x_in_deg")),
		Deg2Rad(config.get_value_for_key_as_double("source_rot_y_in_deg")),
		Deg2Rad(config.get_value_for_key_as_double("source_rot_z_in_deg"))
	);

	const Vector3D source_pos(
		config.get_value_for_key_as_double("source_pos_x_in_m"),
		config.get_value_for_key_as_double("source_pos_y_in_m"),
		config.get_value_for_key_as_double("source_pos_z_in_m")
	);

	HomoTrafo3D Trafo;
	Trafo.set_transformation(source_rot, source_pos);

	PhotonBunch::transform_all_photons_multi_thread(Trafo, photon_bunch);
}
//------------------------------------------------------------------------------
std::vector<Photon*>* LightSourceFromConfig::get_photons() {
	return photon_bunch;
}