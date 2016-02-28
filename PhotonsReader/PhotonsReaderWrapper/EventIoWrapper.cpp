#include "EventIoWrapper.h"
#include "CorsikaIO/EventIo/PhotonFactory.h"
//------------------------------------------------------------------------------
EventIoWrapper::EventIoWrapper(const string _path): 
	BasicWrapper(_path),
	corsika_run(_path) {}
//------------------------------------------------------------------------------
bool EventIoWrapper::has_still_photons_left()const {
	return corsika_run.has_still_events_left();
}
//------------------------------------------------------------------------------
vector<Photon*>* EventIoWrapper::next(Random::Generator* prng) {
		
	EventIo::Event event = corsika_run.next_event();

	vector<Photon*>* photons = new vector<Photon*>;
    uint photon_id = 0;
    for(vector<float> corsika_photon: event.photons) {
        
        EventIo::PhotonFactory cpf(corsika_photon, photon_id++, prng);

        if(cpf.passed_atmosphere())
            photons->push_back(cpf.get_photon());
    }
    return photons;
}