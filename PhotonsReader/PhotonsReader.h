//=================================
// include guard
#ifndef __PhotonsReader_H_INCLUDED__
#define __PhotonsReader_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PhotonsReaderWrapper/BasicWrapper.h"
#include <vector>
#include <string>

//=================================
// read in photons from any file
// All known formats of the mctracer will try to read in the file.
// Only the raw photon information is provided here.
// When reading e.g. Air shower cherenkov photons, and one wants to access the
// Monte Carlo truth aswell, you can not use this reader, but the specific 
// reader, which is just wrapped here, itself.

namespace relleums {

class PhotonsReader {

	BasicWrapper *photons_from_file;
public:

	PhotonsReader(const std::string path);
	~PhotonsReader();
	bool has_still_photons_left()const;
	std::vector<Photon> next(Random::Generator* prng);
};

}  // namespace relleums

#endif // __PhotonsReader_H_INCLUDED__ 
