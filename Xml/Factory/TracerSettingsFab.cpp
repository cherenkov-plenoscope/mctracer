#include "TracerSettingsFab.h"
namespace Xml {
	namespace Configs {
//------------------------------------------------------------------------------
TracerSettings get_TracerSettings_from_node(Xml::Node node) {

	TracerSettings vc;
	vc.pseudo_random_number_seed = Xml::att2int(node, "pseudo_random_number_seed");
	vc.max_number_of_interactions_per_photon = Xml::att2int(node, "max_number_of_interactions_per_photon");
	vc.store_only_final_intersection = Xml::att2bool(node, "store_only_final_intersection");
	vc.use_multithread_when_possible = Xml::att2bool(node, "use_multithread_when_possible");
	return vc;
}
//------------------------------------------------------------------------------
std::string to_node(const TracerSettings &settings) {

	std::stringstream out;
	out << "<settings\n";
	out << "    " << av("pseudo_random_number_seed", std::to_string(settings.pseudo_random_number_seed)) << "\n";
	out << "    " << av("max_number_of_interactions_per_photon", std::to_string(settings.max_number_of_interactions_per_photon)) << "\n";
	out << "    " << av("store_only_final_intersection", Xml::to_string(settings.store_only_final_intersection)) << "\n";
	out << "    " << av("use_multithread_when_possible", Xml::to_string(settings.use_multithread_when_possible)) << "\n";
	out << "/>\n";
	return out.str();
}
//------------------------------------------------------------------------------
	}
}