//=================================
// include guard
#ifndef __PhotonsReaderInternalLightSourceWrapper_H_INCLUDED__
#define __PhotonsReaderInternalLightSourceWrapper_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "BasicWrapper.h"
using std::string;
using std::vector;

//=================================
class InternalPhotonSourceWrapper: public BasicWrapper {

	bool the_single_event_was_already_read;
	vector<Photon*>* photons;
public:

	InternalPhotonSourceWrapper(const string path);
	bool has_still_photons_left()const;
	vector<Photon*>* next(Random::Generator* prng);	
};
#endif // __PhotonsReaderInternalLightSourceWrapper_H_INCLUDED__ 
