#include "AsciiIoWrapper.h"
#include <utility>
#include "merlict/merlict.h"


namespace merlict {

AsciiIoWrapper::AsciiIoWrapper(const std::string _path):
	BasicWrapper(_path)
{
	the_single_event_was_already_read = false;
	photons = raw_matrix2photons(tsvio::gen_table_from_file(path));
}

bool AsciiIoWrapper::has_still_photons_left()const {
	return the_single_event_was_already_read? false: true;
}

std::vector<Photon> AsciiIoWrapper::next(random::Generator* prng) {
	(void)*prng;
	the_single_event_was_already_read = true;
	return photons;
}

}  // namespace merlict
