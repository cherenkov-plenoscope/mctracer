//=================================
// include guard
#ifndef __PhotonsReaderInternalLightSourceWrapper_H_INCLUDED__
#define __PhotonsReaderInternalLightSourceWrapper_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "BasicWrapper.h"
#include <vector>
#include <string>

//=================================
class InternalPhotonSourceWrapper: public BasicWrapper {

	bool the_single_event_was_already_read;
	std::vector<Photon> photons;
public:

	InternalPhotonSourceWrapper(const std::string path);
	bool has_still_photons_left()const;
	std::vector<Photon> next(Random::Generator* prng);	
};
#endif // __PhotonsReaderInternalLightSourceWrapper_H_INCLUDED__ 
