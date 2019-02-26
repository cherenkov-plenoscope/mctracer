//=================================
// include guard
#ifndef __PhotonsReaderAsciiIoWrapper_H_INCLUDED__
#define __PhotonsReaderAsciiIoWrapper_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "BasicWrapper.h"
#include <vector>
#include <string>

//=================================

namespace merlict {

class AsciiIoWrapper: public BasicWrapper {

	bool the_single_event_was_already_read;
	std::vector<Photon> photons;
public:

	AsciiIoWrapper(const std::string path);
	bool has_still_photons_left()const;
	std::vector<Photon> next(random::Generator* prng);	
};

}  // namespace merlict

#endif // __PhotonsReaderAsciiIoWrapper_H_INCLUDED__ 
