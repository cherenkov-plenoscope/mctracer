//=================================
// include guard
#ifndef __LightFieldTelescopePropagation_H_INCLUDE__
#define __LightFieldTelescopePropagation_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include "TracerException.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
#include "CommandLine/cmdline/cmdline.h"

//=================================

namespace LightFieldTelescope {
	
	class Propagation {
		cmdline::parser comand_line_parser;	
		
		const std::string configK = "config";
		const std::string outputK = "output";
		const std::string inputK = "input";
		const std::string nsbK = "nsb";
		const std::string pdeK = "pde";
		const std::string sipm_pulseK = "sipm";

	public:

		Propagation(int argc, char** argv);
		void execute();
	private:
		
		void define_comand_line_keys();
		std::string output_path()const;
		std::string input_path()const;
		std::string config_path()const;
		std::string nsb_vs_wavelength_path()const;
		std::string pde_vs_wavelength_path()const;
		std::string sipm_pulse_template_path()const;
	};
}// LightFieldTelescope
#endif // __LightFieldTelescopePropagation_H_INCLUDE__ 
