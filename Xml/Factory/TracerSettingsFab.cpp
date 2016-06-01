#include "TracerSettingsFab.h"
namespace Xml {
	namespace Configs {
//------------------------------------------------------------------------------
TracerSettings get_TracerSettings_from_node(Xml::Node node) {

	TracerSettings vc;
	vc.pseudo_random_number_seed = node.attribute2int("pseudo_random_number_seed");
	vc.max_number_of_interactions_per_photon = node.attribute2int("max_number_of_interactions_per_photon");
	vc.use_multithread_when_possible = node.attribute2bool("use_multithread_when_possible");
	return vc;
}
//------------------------------------------------------------------------------
std::string to_node(const TracerSettings &settings) {

	std::stringstream out;
	out << "<settings\n";
	out << "    " << av("pseudo_random_number_seed", std::to_string(settings.pseudo_random_number_seed)) << "\n";
	out << "    " << av("max_number_of_interactions_per_photon", std::to_string(settings.max_number_of_interactions_per_photon)) << "\n";
	out << "    " << av("use_multithread_when_possible", Xml::to_string(settings.use_multithread_when_possible)) << "\n";
	out << "/>\n";
	return out.str();
}
//------------------------------------------------------------------------------
	}
}