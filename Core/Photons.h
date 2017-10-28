//=================================
// include guard
#ifndef __PHOTONBUNCH_H_INCLUDED__
#define __PHOTONBUNCH_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Photon.h"
#include "Core/PropagationEnvironment.h"
#include <vector>
#include <string>

//=================================
namespace Photons {

	std::string get_print(const std::vector<Photon> *photons);
	// propagation
	void propagate_photons_in_scenery_with_settings(
		std::vector<Photon> *photons,
		const Frame *world, 
		const PropagationConfig *settings,
		Random::Generator* prng
	);	

	void propagate_photons_using_single_thread(
		std::vector<Photon> *photons,
		const Frame* world, 
		const PropagationConfig* settings,
		Random::Generator* prng
	);

	void propagate_photons_using_multi_thread(
		std::vector<Photon> *photons,
		const Frame* world, 
		const PropagationConfig* settings
	);

	// In Out to raw matrix vec<vec<d>>
	// Can be given to AsciiIO to write/read to/from text file
	std::vector<Photon> raw_matrix2photons(std::vector<std::vector<double> > raw_matrix);
	std::vector<std::vector<double> > photons2raw_matrix(std::vector<Photon> *photons);
	std::vector<double> photon2raw_row(Photon* ph);
	Photon raw_row2photon(std::vector<double> &raw_row);
	void assert_raw_row_size_matches_photon(std::vector<double> &raw_row);

	// creation of photons
	namespace Source {

		std::vector<Photon> parallel_towards_z_from_xy_disc(
			const double disc_radius,
			const uint number_of_photons
		);

		std::vector<Photon> point_like_towards_z_opening_angle_num_photons(
			const double opening_angle,
			const uint number_of_photons 
		);
	}

	void transform_all_photons(const HomTra3 Trafo, std::vector<Photon> *photons);
	void transform_all_photons_multi_thread(
		const HomTra3 Trafo, 
		std::vector<Photon> *photons
	);
}
#endif // __PHOTONBUNCH_H_INCLUDED__