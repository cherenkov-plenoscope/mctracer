//=================================
// include guard
#ifndef __CORSIKATOOLS_H_INCLUDE__
#define __CORSIKATOOLS_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <array>
using std::string;
using std::vector;
using std::array;

//=================================
namespace Corsika {

	namespace EventHeader {
		string get_print(const array<float, 273> &rh);
		float event_number(const array<float, 273> &eh);
		float particle_id(const array<float, 273> &eh);
		float total_energy_in_GeV(const array<float, 273> &eh);
		float zenith(const array<float, 273> &eh);
		float azimuth(const array<float, 273> &eh);
		float first_interaction_height_in_cm(const array<float, 273> &eh);
	}

	namespace RunHeader {
		string get_print(const array<float, 273> &rh);
		float run_number(const array<float, 273> &rh);
		float slope_of_energy_spektrum(const array<float, 273> &rh);
		float energy_range_start(const array<float, 273> &rh);
		float energy_range_end(const array<float, 273> &rh);
		float number_of_observation_levels(const array<float, 273> &rh);
		float observation_level_at(const array<float, 273> &rh, const uint i);
	}

	float str2float(const string word);
	string float2str(const float word_in_float);
}
#endif // __CORSIKATOOLS_H_INCLUDE__ 
