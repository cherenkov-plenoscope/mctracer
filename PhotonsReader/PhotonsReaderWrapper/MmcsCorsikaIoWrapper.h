//=================================
// include guard
#ifndef __PhotonsReaderMmcsCorsikaIoWrapper_H_INCLUDED__
#define __PhotonsReaderMmcsCorsikaIoWrapper_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CorsikaIO/MmcsCorsikaFullEventGetter.h"
#include "BasicWrapper.h"
using std::string;
using std::vector;

//=================================
class MmcsCorsikaIoWrapper: public BasicWrapper {

	mutable MmcsCorsikaFullEventGetter corsika_run;
	mutable MmcsCorsikaEvent event;
public:

	MmcsCorsikaIoWrapper(const string path);
	bool has_still_photons_left()const;
	vector<Photon*>* next(Random::Generator* prng);	
};
#endif // __PhotonsReaderMmcsCorsikaIoWrapper_H_INCLUDED__ 
