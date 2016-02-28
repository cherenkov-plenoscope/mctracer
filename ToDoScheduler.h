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
using std::string;
using std::cout;
//=================================
class ToDoScheduler {

	CommandLine::Parser cmd;	
public:
	const string kscenery = "scenery";

	ToDoScheduler(int argc, char** argv);
	void execute();
private:
	
	void define_commands();
	void propagate_photons_through_scenery()const; 
	
	const string scenery_path()const;
	const string photon_path()const;
	const string output_path()const;
	const string config_path()const;
	string help()const;

	string help_show_scenery()const;
	bool command_is_complete_show_scenery()const;
	void show_scenery()const;

	string help_propagate()const;
	bool command_is_complete_propagate()const;
	void propagate()const;


	string help_investigate_events()const;
	bool command_is_complete_investigate_events()const;
	void investigate_events()const;
};
#endif // __ToDoScheduler_H_INCLUDE__ 