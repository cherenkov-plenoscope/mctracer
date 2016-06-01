//=================================
// include guard
#ifndef __EventIoPhotonFactory_H_INCLUDED__
#define __EventIoPhotonFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <sstream>
#include "Core/Photon.h"
#include "Core/Random/Random.h"
#include <array>
using std::string;
using std::stringstream;
using std::vector;
using std::array;
//=================================
namespace EventIo {

	class PhotonFactory {

		static const uint x_pos_idx = 0;
		static const uint y_pos_idx = 1;
		static const uint x_dir_cos_idx = 2;
		static const uint y_dir_cos_idx = 3;
		static const uint time_idx = 4; 
		static const uint z_emission_height_idx = 5;
		static const uint photon_weight_idx = 6;
		static const uint wavelength_idx = 7;

		bool passed_through_atmosphere_flag = false;
		uint id;
		array<float, 8> corsika_photon;
	public:

		class BadCorsikaPhotonShape : public TracerException {
			using TracerException::TracerException;
		};

		class BadPhotonWeight : public TracerException {
			using TracerException::TracerException;
		};	

		PhotonFactory(
			const array<float, 8>& _corsika_photon, 
			const uint id,
			Random::Generator *prng
		);
		bool passed_atmosphere()const;
		Photon* get_photon();
		Vec3 causal_get_direction()const;
		Vec3 intersection_with_xy_floor_plane()const;
		double ray_parameter_for_production_point()const;
		double x_pos_on_xy_plane_in_m()const;
		double y_pos_on_xy_plane_in_m()const;
		double production_height_in_m()const;
		double wavelength_in_m()const;
		double production_distance_offset()const;
		double relative_arrival_time_on_ground()const;
		float x_pos_on_world_x_y_plane_in_cm()const;
		float y_pos_on_world_x_y_plane_in_cm()const;
		float wavelength_in_nm()const;
		float production_height_in_cm()const;
		float photon_survival_probability()const;
	private:

		void check_once_if_passed_atmosphere(Random::Generator *prng);
		void assert_corsika_photon_has_correct_length()const;
		void assert_photon_weight_is_between_zero_and_one()const;
	};
}// EventIo
#endif // __EventIoPhotonFactory_H_INCLUDED__ 
