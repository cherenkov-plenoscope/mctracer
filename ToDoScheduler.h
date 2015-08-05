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
#include "TracerException.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
#include "Core/TimeStamp.h"
#include "cmdline/cmdline.h"
#include "WorldFactory.h"
#include "Cameras/FreeOrbitCamera.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaFullEventGetter.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaPhotonData.h"

//=================================
class ToDoScheduler {
	cmdline::parser comand_line_parser;	
	
	const std::string render_key = "render";
	const std::string geometry_key = "geometry";
	const std::string propagate_key = "propagate";
	const std::string photons_key = "photons";
	const std::string investigation_key = "investigate";
public:

	ToDoScheduler(int argc, char** argv);
	void execute();
private:
	
	void define();
	void render_geometry()const;
	void propagate_photons_through_geometry()const; 
	void investigate_single_photon_propagation_in_geometry()const;
	const std::string get_geometry_file()const;
	const std::string get_photon_file()const;
};
#endif // __ToDoScheduler_H_INCLUDE__ 