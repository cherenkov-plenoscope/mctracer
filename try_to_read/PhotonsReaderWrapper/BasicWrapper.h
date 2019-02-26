//=================================
// include guard
#ifndef __PhotonsReaderBasicWrapper_H_INCLUDED__
#define __PhotonsReaderBasicWrapper_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "merlict/Photons.h"
#include "merlict/random/random.h"
#include <vector>
#include <string>

namespace merlict {
//=================================
// pure virtual
// serves only as API template for the other actual wrappers
class BasicWrapper {
protected:
	const std::string path;
public:

	BasicWrapper(const std::string path);
	virtual ~BasicWrapper() {};
	virtual bool has_still_photons_left()const=0;
	virtual std::vector<Photon> next(random::Generator* prng)=0;
};

}  // namespace merlict

#endif // __PhotonsReaderBasicWrapper_H_INCLUDED__ 
