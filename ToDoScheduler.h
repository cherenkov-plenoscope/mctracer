//=================================
// include guard
#ifndef __ToDoScheduler_H_INCLUDE__
#define __ToDoScheduler_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "TracerException.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
#include "Core/TimeStamp.h"
#include "cmdline/cmdline.h"
#include "Cameras/FreeOrbitCamera.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaFullEventGetter.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaPhotonData.h"
#include "TelescopeArrayControl.h"
#include "LightSourceFromConfig.h"

//=================================
class ToDoScheduler {
	cmdline::parser comand_line_parser;	
	
	const std::string render_key = "render";
	const std::string propagate_key = "propagate";
	const std::string investigation_key = "investigate";
	const std::string pointsource_key = "lightsource";
	const std::string geometry_key = "geometry";
	const std::string photons_key = "photons";
	const std::string config_key = "config";
	const std::string output_key = "output";
public:

	ToDoScheduler(int argc, char** argv);
	void execute();
private:
	
	void define();
	void render_geometry()const;
	void propagate_photons_through_geometry()const; 
	void investigate_single_photon_propagation_in_geometry()const;
	void point_spread_investigation_in_geometry()const;
	const std::string get_geometry_file()const;
	const std::string get_photon_file_name()const;
	const std::string get_output_file_name()const;
	const std::string get_config_file_name()const;
};
#endif // __ToDoScheduler_H_INCLUDE__ 