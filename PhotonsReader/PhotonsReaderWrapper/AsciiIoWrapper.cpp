#include "AsciiIoWrapper.h"
#include "Tools/AsciiIo.h"
#include <utility>
using std::string;
using std::vector;

namespace relleums {
//------------------------------------------------------------------------------
AsciiIoWrapper::AsciiIoWrapper(const string _path): 
	BasicWrapper(_path)
{
	the_single_event_was_already_read = false;
	photons = Photons::raw_matrix2photons(tsvio::gen_table_from_file(path));
}
//------------------------------------------------------------------------------
bool AsciiIoWrapper::has_still_photons_left()const {
	return the_single_event_was_already_read? false: true;
}
//------------------------------------------------------------------------------
vector<Photon> AsciiIoWrapper::next(random::Generator* prng) {
	(void)*prng;
	the_single_event_was_already_read = true;
	return photons;
}
//------------------------------------------------------------------------------
}  // namespace relleums
