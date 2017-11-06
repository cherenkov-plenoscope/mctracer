//=================================
// include guard
#ifndef __MMCSCORSIKAPHOTONDATA_H_INCLUDE__
#define __MMCSCORSIKAPHOTONDATA_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <vector>
#include <math.h>
#include <sstream>
#include "Tools/StringTools.h"
#include "MmcsCorsikaTools.h"
#include "MmcsCorsikaSubBlock.h"
//=================================
class MmcsCorsikaPhotonData {

	std::vector<std::vector<float>> photon_matrix;
public:
	void append_valid_photons_from_sub_block(
		const MmcsCorsikaSubBlock &sub_block
	);
	void print()const;
	std::string str()const;
	unsigned int number_of_photons()const;

	int get_parent_code(const unsigned int at)const;
	float get_production_height_in_cm(const unsigned int at)const;
	float get_x_pos_on_world_x_y_plane_in_cm(const unsigned int at)const;
	float get_y_pos_on_world_x_y_plane_in_cm(const unsigned int at)const;
	float get_cosine_between_dir_and_world_x_axis(const unsigned int at)const;
	float get_cosine_between_dir_and_world_y_axis(const unsigned int at)const;
	float get_wavelength_in_nm(const unsigned int at)const;
	float get_production_time_in_ns(const unsigned int at)const;
	std::string get_photon_print_at(const unsigned int at)const;
	std::string get_statistics_print()const;
private:
	bool row_has_only_zeros(std::vector<float> &photon_row)const;
	void access_is_in_range(const unsigned int at)const;
	int get_code_holder(const unsigned int at)const;
	std::string get_matrix_print()const;
};
#endif // __MMCSCORSIKAPHOTONDATA_H_INCLUDE__ 
