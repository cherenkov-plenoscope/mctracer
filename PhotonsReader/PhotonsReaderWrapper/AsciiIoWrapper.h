//=================================
// include guard
#ifndef __PhotonsReaderAsciiIoWrapper_H_INCLUDED__
#define __PhotonsReaderAsciiIoWrapper_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "BasicWrapper.h"
using std::string;
using std::vector;

//=================================
class AsciiIoWrapper: public BasicWrapper {

	bool the_single_event_was_already_read;
	vector<Photon*>* photons;
public:

	AsciiIoWrapper(const string path);
	bool has_still_photons_left()const;
	vector<Photon*>* next(Random::Generator* prng);	
};
#endif // __PhotonsReaderAsciiIoWrapper_H_INCLUDED__ 
