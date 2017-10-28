#include "InternalPhotonSourceWrapper.h"
#include "Xml/Factory/InternalPhotonSource.h"
//------------------------------------------------------------------------------
InternalPhotonSourceWrapper::InternalPhotonSourceWrapper(const string _path): 
	BasicWrapper(_path)
{
	the_single_event_was_already_read = false;

	Xml::Document doc(path);
	Xml::Node doc_node = doc.node();
	Xml::Node light_source_node = doc_node.child("lightsource");
	photons = Xml::InternalPhotonSource::node2photons(light_source_node);
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