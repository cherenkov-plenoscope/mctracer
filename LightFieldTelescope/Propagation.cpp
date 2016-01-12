#include "Propagation.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
Propagation::Propagation(int argc, char** argv) {

	define_comand_line_keys();
	comand_line_parser.parse_check(argc, argv);
	execute();
}
//------------------------------------------------------------------------------
void Propagation::define_comand_line_keys() {

	comand_line_parser.add<std::string>(
		configK, 'c' ,"configuration file controling the simulation" , false, ""
	);

	comand_line_parser.add<std::string>(
		outputK, 'o', "output path", false, ""
	);

	comand_line_parser.add<std::string>(
		inputK, 'i', "input path for CORSIKA Cherenkov photons", false, ""
	);
}
//------------------------------------------------------------------------------
void Propagation::execute() {
	std::cout << "coming soon :)\n";
}
//------------------------------------------------------------------------------
}// LightFieldTelescope