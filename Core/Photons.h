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
using std::string;
using std::stringstream;
using std::vector;

//=================================
namespace Photons {

	string get_print(const vector<Photon*> *photons);
	// propagation
	void propagate_photons_in_world_with_settings(
		vector<Photon*> *photons,
		const Frame *world, 
		const TracerSettings *settings,
		Random::Generator* prng
	);	

	void propagate_photons_using_single_thread(
		vector<Photon*> *photons,
		const Frame* world, 
		const TracerSettings* settings,
		Random::Generator* prng
	);

	void propagate_photons_using_multi_thread(
		vector<Photon*> *photons,
		const Frame* world, 
		const TracerSettings* settings
	);

	// In Out to raw matrix vec<vec<d>>
	// Can be given to AsciiIO to write/read to/from text file
	vector<Photon*> *raw_matrix2photons(vector<vector<double> > raw_matrix);
	vector<vector<double> > photons2raw_matrix(vector<Photon*> *photons);
	vector<double> photon2raw_row(Photon* ph);
	Photon* raw_row2photon(vector<double> &raw_row);
	void assert_raw_row_size_matches_photon(vector<double> &raw_row);
	// Trajectories
	class Trajectories {
		uint number_of_trajectories_handed_out_already;
		vector<Photon*> *photons;
		const TracerSettings *settings;
	public:
		Trajectories(
			vector<Photon*> *_photon_bunch, 
			const TracerSettings *settings
		);
		bool has_still_trajectories_left()const;
		Frame* get_next_trajectoy();
	};

	// creation of photons
	namespace Source {

		vector<Photon*> *parallel_towards_z_from_xy_disc(
			const double disc_radius,
			const uint number_of_photons
		);

		vector<Photon*> *point_like_towards_z_opening_angle_num_photons(
			const double opening_angle,
			const uint number_of_photons 
		);
	}

	void transform_all_photons(const HomTra3 Trafo, vector<Photon*> *photons);
	void transform_all_photons_multi_thread(
		const HomTra3 Trafo, 
		vector<Photon*> *photons
	);

	// delete all history
	void delete_photons(vector<Photon*> *photons);
}

#endif // __PHOTONBUNCH_H_INCLUDED__
