//=================================
// include guard
#ifndef __PhotonsReaderBasicWrapper_H_INCLUDED__
#define __PhotonsReaderBasicWrapper_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Photons.h"
#include "Core/Random/Random.h"
using std::string;
using std::vector;

//=================================
// pure virtual
// serves only as API template for the other actual wrappers
class BasicWrapper {
protected:
	const string path;
public:

	BasicWrapper(const string path);
	virtual ~BasicWrapper() {};
	virtual bool has_still_photons_left()const=0;
	virtual vector<Photon*>* next(Random::Generator* prng)=0;
};

#endif // __PhotonsReaderBasicWrapper_H_INCLUDED__ 
