#include "MmcsCorsikaIoWrapper.h"
using std::cout;
//------------------------------------------------------------------------------
MmcsCorsikaIoWrapper::MmcsCorsikaIoWrapper(const string _path): 
	BasicWrapper(_path),
	corsika_run(_path)
{
	if(corsika_run.has_still_events_left())
		event = corsika_run.get_next_event();
}
//------------------------------------------------------------------------------
bool MmcsCorsikaIoWrapper::has_still_photons_left()const {

	if(event.can_be_reused_again()) {
		return true;
	}
	else if(corsika_run.has_still_events_left()){
		event = corsika_run.get_next_event();

		return has_still_photons_left();
	}
	else{
		return false;
	}
}
//------------------------------------------------------------------------------
vector<Photon*>* MmcsCorsikaIoWrapper::next(Random::Generator* prng) {
	return event.use_once_more_and_get_photons();
}
//------------------------------------------------------------------------------