#include "PropagationConfigFab.h"
#include <sstream>
namespace Xml {
	namespace Configs {
//------------------------------------------------------------------------------
PropagationConfig get_PropagationConfig_from_node(Xml::Node node) {

	PropagationConfig vc;
	vc.max_number_of_interactions_per_photon = node.to_int("max_number_of_interactions_per_photon");
	vc.use_multithread_when_possible = node.to_bool("use_multithread_when_possible");
	return vc;
}
//------------------------------------------------------------------------------
std::string to_node(const PropagationConfig &settings) {

	std::stringstream out;
	out << "<settings\n";
	out << "    " << av("max_number_of_interactions_per_photon", std::to_string(settings.max_number_of_interactions_per_photon)) << "\n";
	out << "    " << av("use_multithread_when_possible", Xml::to_string(settings.use_multithread_when_possible)) << "\n";
	out << "/>\n";
	return out.str();
}
//------------------------------------------------------------------------------
	}
}