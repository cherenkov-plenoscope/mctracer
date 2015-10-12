//=================================
// include guard
#ifndef __LightSourceFromConfig_H_INCLUDED__
#define __LightSourceFromConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/PhotonBunch.h"
#include "ProgramOptions.h"
//=================================
class LightSourceFromConfig{

	std::vector<Photon*>* photon_bunch;
	KeyValueMap config;
public:
	LightSourceFromConfig(KeyValueMap config);
	std::vector<Photon*>* get_photons();
private:
	void transform_photons();
	void point_like_source();
	void parallel_from_disc();
	void unknown_type();
};
#endif // __LightSourceFromConfig_H_INCLUDED__ 
