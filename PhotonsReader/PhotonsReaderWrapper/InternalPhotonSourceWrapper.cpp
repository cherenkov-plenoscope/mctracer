#include "InternalPhotonSourceWrapper.h"
#include "json.h"
using std::string;
using std::vector;

namespace relleums {
//------------------------------------------------------------------------------
InternalPhotonSourceWrapper::InternalPhotonSourceWrapper(const string _path): 
	BasicWrapper(_path)
{
	the_single_event_was_already_read = false;
	photons = json::load_photons(path);
}
//------------------------------------------------------------------------------
bool InternalPhotonSourceWrapper::has_still_photons_left()const {
	return the_single_event_was_already_read? false: true;
}
//------------------------------------------------------------------------------
vector<Photon> InternalPhotonSourceWrapper::next(Random::Generator* prng) {
	(void)*prng;
	the_single_event_was_already_read = true;
	return photons;
}
//------------------------------------------------------------------------------
}  // namespace relleums
