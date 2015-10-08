//=================================
// include guard
#ifndef __PHOTONBUNCH_H_INCLUDED__
#define __PHOTONBUNCH_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include "Photon.h"
#include <vector>

//=================================
namespace PhotonBunch {

	// propagation
	void propagate_photons_in_world_with_settings(
		std::vector<Photon*> *photon_bunch,
		const Frame *world, 
		const TracerSettings *settings
	);	

	void propagate_photons_using_single_thread(
		std::vector<Photon*> *photon_bunch,
		const Frame* world, 
		const TracerSettings* settings
	);

	void propagate_photons_using_multi_thread(
		std::vector<Photon*> *photon_bunch,
		const Frame* world, 
		const TracerSettings* settings
	);

	// In Out to raw matrix vec<vec<d>>
	// Can be given to AsciiIO to write/read to/from text file
	std::vector<Photon*> *raw_matrix2photons(
		std::vector<std::vector<double>> raw_matrix
	);

	std::vector<std::vector<double>> photons2raw_matrix(
		std::vector<Photon*> *photon_bunch
	);

	std::vector<double> photon2raw_row(Photon* ph);

	Photon* raw_row2photon(std::vector<double> &raw_row);
	
	void assert_raw_row_size_matches_photon(std::vector<double> &raw_row);

	// Trajectories
	class Trajectories {
		uint number_of_trajectories_handed_out_already;
		std::vector<Photon*> *photon_bunch;
	public:
		Trajectories(std::vector<Photon*> *_photon_bunch);
		bool has_still_trajectories_left()const;
		Frame* get_next_trajectoy();
	};

	// probing and testing
	uint get_number_of_photnons_absorbed_in_object(
		const std::vector<Photon*> *photon_bunch, 
		const Frame* frame
	);

	// creation of photons
	namespace Source {

		std::vector<Photon*> *point_like_towards_z_opening_angle_num_photons(
			const double opening_angle,
			const uint number_of_photons 
		);
	}

	void transform_all_photons(
		const HomoTrafo3D Trafo, 
		std::vector<Photon*> *photon_bunch
	);

	void transform_all_photons_multi_thread(
		const HomoTrafo3D Trafo, 
		std::vector<Photon*> *photon_bunch
	);

	// delete all history
	void delete_photons_and_history(std::vector<Photon*> *photon_bunch);
}

#endif // __PHOTONBUNCH_H_INCLUDED__
