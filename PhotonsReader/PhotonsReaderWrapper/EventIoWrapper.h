//=================================
// include guard
#ifndef __PhotonsReaderEventIoWrapper_H_INCLUDED__
#define __PhotonsReaderEventIoWrapper_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "BasicWrapper.h"
#include "Corsika/EventIo/EventIo.h"
using std::string;
using std::vector;

//=================================
class EventIoWrapper: public BasicWrapper {

	EventIo::Run corsika_run;
public:

	EventIoWrapper(const string path);
	bool has_still_photons_left()const;
	vector<Photon*>* next(Random::Generator* prng);	
};
#endif // __PhotonsReaderEventIoWrapper_H_INCLUDED__ 
