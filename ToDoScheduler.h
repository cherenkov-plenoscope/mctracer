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
#include "CommandLine/CommandLine.h"
#include "Cameras/FlyingCamera.h"
#include "CorsikaIO/MmcsCorsikaFullEventGetter.h"
#include "CorsikaIO/MmcsCorsikaPhotonData.h"
#include "TelescopeArrayControl.h"
#include "LightSourceFromConfig.h"
using std::string;
using std::cout;
//=================================
class ToDoScheduler {
	CommandLine cmd;	
	
	const string render_key = "render";
	const string propagate_key = "propagate";
	const string investigation_key = "investigate";
	const string pointsource_key = "lightsource";
	const string geometry_key = "geometry";
	const string photons_key = "photons";
	const string config_key = "config";
	const string output_key = "output";
	const string skydome_key = "skydome";
public:

	ToDoScheduler(int argc, char** argv);
	void execute();
private:
	
	void define();
	void render_geometry()const;
	void propagate_photons_through_geometry()const; 
	void investigate_single_photon_propagation_in_geometry()const;
	void point_spread_investigation_in_geometry()const;
	const string geometry_path()const;
	const string photon_path()const;
	const string output_path()const;
	const string config_path()const;
	const string skydome_path()const;
};
#endif // __ToDoScheduler_H_INCLUDE__ 