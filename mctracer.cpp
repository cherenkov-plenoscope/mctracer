#include <iostream> 
#include <string>
#include <sstream>

#include "WorldFactory.h"
#include "Vector3D.h"
#include "Tools/Tools.h"
//====================
#include "Ray.h"
#include "Photon.h"
//====================
#include "ListOfPropagations.h"
#include "Cameras/FreeOrbitCamera.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaFullEventGetter.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaPhotonData.h"

int main(int argc, char* argv[]) {
	try{
		UserInteraction::print_welcome_screen();

		GlobalSettings settings;		
		
		std::string file_chosen_by_user = 
			UserInteraction::input("Enter a world file to load: ");

		WorldFactory file2world;
		file2world.load(file_chosen_by_user);
		CartesianFrame *Mworld = file2world.world();

		/*
		//string filename = UserInteraction::input("Enter Mmcs CORSIKA file: ");
		std::string filename = "../../cer002600";
		MmcsCorsikaFullEventGetter event_getter(filename);

		while(event_getter.has_still_events_left()) {

			MmcsCorsikaEvent event = event_getter.get_next_event();	

			//std::cout << event.get_print() << std::endl;
			while(event.can_be_reused_again()) {
				
				ListOfPropagations *photons = 
					event.use_once_more_and_get_mctracer_photons();
					
				photons->propagate_in_world_with_settings(Mworld, &settings);
			}
		}*/

		FreeOrbitCamera free(Mworld, &settings);
	}catch(std::exception &error){
		std::cerr << error.what(); 
	}	
	return 0;
}