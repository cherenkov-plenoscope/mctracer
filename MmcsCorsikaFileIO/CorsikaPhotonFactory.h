//=================================
// include guard
#ifndef __CorsikaPhotonFactory_H_INCLUDED__
#define __CorsikaPhotonFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <sstream>
#include "Core/Photon.h"

//=================================
class CorsikaPhotonFactory {

	const uint x_pos_idx = 0;
	const uint y_pos_idx = 1;
	const uint x_dir_cos_idx = 2;
	const uint y_dir_cos_idx = 3;
	const uint time_idx = 4; 
	const uint z_emission_height_idx = 5;
	const uint photon_weight_idx = 6;
	const uint wavelength_idx = 7;

	PseudoRandomNumberGenerator *prng;
	bool passed_through_atmosphere_flag = false;

	uint id;
	std::vector<float> corsika_photon;

	class BadCorsikaPhotonShape : public TracerException {
		using TracerException::TracerException;
	};	
public:

	CorsikaPhotonFactory(
		const std::vector<float>& _corsika_photon, 
		const uint id,
		PseudoRandomNumberGenerator *prng
	);
	bool passed_atmosphere()const;
	Photon* get_photon();
	double x_core_position_in_m = 0.0;
	double y_core_position_in_m = 0.0;

	Vector3D causal_direction()const;
	Vector3D intersection_with_xy_floor_plane()const;
	double x_pos_on_xy_plane_in_m()const;
	double y_pos_on_xy_plane_in_m()const;
	double production_height_in_m()const;
	double wavelength_in_m()const;
	bool row_has_only_zeros()const;
	int parent_code()const;
	float x_pos_on_world_x_y_plane_in_cm()const;
	float y_pos_on_world_x_y_plane_in_cm()const;
	float cosine_between_dir_and_world_x_axis()const;
	float cosine_between_dir_and_world_y_axis()const;
	float wavelength_in_nm()const;
	float production_time_in_ns()const;
	float production_height_in_cm()const;
	float photon_survival_probability()const;
private:

	void check_once_if_passed_atmosphere();
	void assert_corsika_photon_has_correct_length();
};
#endif // __CorsikaPhotonFactory_H_INCLUDED__ 
