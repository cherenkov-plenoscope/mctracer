//=================================
// include guard
#ifndef __PHOTON_MC_TRUTH_H_INCLUDED__
#define __PHOTON_MC_TRUTH_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <string>
//=================================
struct PhotonMcTruth {
	double production_height_over_sea_level;
	static const PhotonMcTruth void_truth;

	PhotonMcTruth();

	double get_production_height_over_sea_level() const;
	std::string get_print() const;
};

#endif // __PHOTON_MC_TRUTH_H_INCLUDED__
