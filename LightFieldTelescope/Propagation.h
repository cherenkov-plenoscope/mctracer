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
#include "CommandLine/CommandLine.h"
using std::string;
using std::cout;
//=================================

namespace LightFieldTelescope {
	
	class Propagation {
		CommandLine cmd;	
		
		const string configK = "config";
		const string outputK = "output";
		const string inputK = "input";
		const string nsbK = "nsb";
		const string pdeK = "pde";
		const string sipm_pulseK = "sipm";

	public:

		Propagation(int argc, char** argv);
		void execute();
	private:
		
		void define_comand_line_keys();
		string output_path()const;
		string input_path()const;
		string config_path()const;
		string nsb_vs_wavelength_path()const;
		string pde_vs_wavelength_path()const;
		string sipm_pulse_template_path()const;
	};
}// LightFieldTelescope
#endif // __LightFieldTelescopePropagation_H_INCLUDE__ 
