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
#include <sstream>

//=================================
namespace PhotonBunch {

	std::vector<Photon> raw_matrix2photons(
		std::vector<std::vector<double>> &raw_matrix
	);
	std::vector<std::vector<double>> photons2raw_matrix(
		std::vector<Photon> &photon_bunch
	);
	std::vector<double> photon2raw_row(Photon &ph);
	Photon raw_row2photon(std::vector<double> &raw_row);
	void assert_raw_row_size_matches_photon(std::vector<double> &raw_row);
}

#endif // __PHOTONBUNCH_H_INCLUDED__
