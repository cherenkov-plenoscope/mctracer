//=================================
// include guard
#ifndef __PhotonsReaderMmcsCorsikaIoWrapper_H_INCLUDED__
#define __PhotonsReaderMmcsCorsikaIoWrapper_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Corsika/MagicMonteCarloSimulation/MmcsCorsikaFullEventGetter.h"
#include "BasicWrapper.h"
#include <vector>
#include <string>

//=================================
class MmcsCorsikaIoWrapper: public BasicWrapper {

	mutable MmcsCorsikaFullEventGetter corsika_run;
	mutable MmcsCorsikaEvent event;
public:

	MmcsCorsikaIoWrapper(const std::string path);
	bool has_still_photons_left()const;
	std::vector<Photon> next(Random::Generator* prng);	
};
#endif // __PhotonsReaderMmcsCorsikaIoWrapper_H_INCLUDED__ 
