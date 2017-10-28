#include "MmcsCorsikaPhotonData.h"
//------------------------------------------------------------------------------
void MmcsCorsikaPhotonData::print()const {
	std::cout << get_statistics_print();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaPhotonData::get_statistics_print()const {
	std::stringstream out;
	out << "__Photon_Data__\n";
	out << " number_of_photons: " << number_of_photons() << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaPhotonData::str()const {
	std::stringstream out;
	out << "Photon Matrix\n" << get_matrix_print();
	return out.str();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaPhotonData::get_matrix_print()const {
	std::stringstream out;

	for(std::vector<float> row : photon_matrix)
		out << MmcsCorsikaTools::float_vec_2_str_using_delimiter(row," ") << "\n";

	return out.str();
}
//------------------------------------------------------------------------------
void MmcsCorsikaPhotonData::append_valid_photons_from_sub_block(
	const MmcsCorsikaSubBlock &sub_block
) {
	for(uint row=0; row<sub_block.rows(); row++) {
		
		std::vector<float> photon_row;
		for(uint col=0; col<sub_block.cols(); col++) {

			const uint sub_block_position = row*sub_block.cols()+col;
			photon_row.push_back(sub_block[sub_block_position]);
		}

		if(!row_has_only_zeros(photon_row))
			photon_matrix.push_back(photon_row);
	}
}
//------------------------------------------------------------------------------
bool MmcsCorsikaPhotonData::row_has_only_zeros(std::vector<float> &photon_row)const {
	for(float element : photon_row)
		if(element != 0.0)
			return false;
	return true;
}
//------------------------------------------------------------------------------
uint MmcsCorsikaPhotonData::number_of_photons()const {
	return photon_matrix.size();
}
//------------------------------------------------------------------------------
void MmcsCorsikaPhotonData::access_is_in_range(const uint at)const {
	if( at > number_of_photons() ) {
		std::stringstream info;
		info << "MmcsCorsikaPhotonData:\n";
		info << "Tried to access photon number " << at << " but there are only";
		info << " " << number_of_photons() << " in this set.\n";
		throw std::runtime_error(info.str());	
	}
}
//------------------------------------------------------------------------------
int MmcsCorsikaPhotonData::get_parent_code(const uint at)const {
	access_is_in_range(at);
	return get_code_holder(at)/100000;
}
//------------------------------------------------------------------------------
float MmcsCorsikaPhotonData::get_x_pos_on_world_x_y_plane_in_cm(const uint at)const {
	access_is_in_range(at);
	return photon_matrix.at(at).at(1);
}
//------------------------------------------------------------------------------
float MmcsCorsikaPhotonData::get_y_pos_on_world_x_y_plane_in_cm(const uint at)const {
	access_is_in_range(at);
	return photon_matrix.at(at).at(2);
}
//------------------------------------------------------------------------------
float MmcsCorsikaPhotonData::get_cosine_between_dir_and_world_x_axis(const uint at)const {
	access_is_in_range(at);
	return photon_matrix.at(at).at(3);
}
//------------------------------------------------------------------------------
float MmcsCorsikaPhotonData::get_cosine_between_dir_and_world_y_axis(const uint at)const {
	access_is_in_range(at);
	return photon_matrix.at(at).at(4);
}
//------------------------------------------------------------------------------
float MmcsCorsikaPhotonData::get_wavelength_in_nm(const uint at)const {
	access_is_in_range(at);
	return fmod(photon_matrix.at(at).at(0),1000.0);
}
//------------------------------------------------------------------------------
float MmcsCorsikaPhotonData::get_production_time_in_ns(const uint at)const {
	access_is_in_range(at);
	return photon_matrix.at(at).at(5);
}
//------------------------------------------------------------------------------
float MmcsCorsikaPhotonData::get_production_height_in_cm(const uint at)const {
	access_is_in_range(at);
	return photon_matrix.at(at).at(6);
}
//------------------------------------------------------------------------------
int MmcsCorsikaPhotonData::get_code_holder(const uint at)const {
	access_is_in_range(at);
	return int(round(photon_matrix.at(at).at(0)));
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaPhotonData::get_photon_print_at(const uint at)const {
	std::stringstream out;
	out << "wavel" << get_wavelength_in_nm(at) << "nm ";
	out << "parent" << get_parent_code(at) << " ";
	out << "x_pos" << get_x_pos_on_world_x_y_plane_in_cm(at) << "cm ";
	out << "y_pos" << get_y_pos_on_world_x_y_plane_in_cm(at) << "cm ";
	out << "time" << get_production_time_in_ns(at) << "ns ";
	out << "height" << get_production_height_in_cm(at) << "cm ";
	out << "\n";
	return out.str();
}
//------------------------------------------------------------------------------