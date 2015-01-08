#include <iostream> 
#include <string>
#include <sstream>

#include "WorldFactory.h"
#include "Vector3D.h"
#include "Functions.h"
//====================
#include "Ray.h"
#include "Photon.h"
//====================
#include "ListOfPropagations.h"
#include "Cameras/FreeOrbitCamera.h"
#include "CsvHandler.h"

#include "MmcsCorsikaFileIO/MmcsCorsikaFullEventGetter.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaPhotonData.h"

int main(){
	try{
		UserInteraction::print_welcome_screen();
		
		GlobalSettings settings;		
		settings.set_max_number_of_reflections(5);
		settings.set_csv_decimal_presicion(9);
		
		std::string file_chosen_by_user = 
			UserInteraction::input("Enter a world file to load: ");

		WorldFactory file2world;
		file2world.load(file_chosen_by_user);
		CartesianFrame *Mworld = file2world.world();

		string filename = UserInteraction::input("Enter Mmcs CORSIKA file: ");
		MmcsCorsikaFullEventGetter event_getter(filename);

		while(event_getter.has_still_events_left()) {

			MmcsCorsikaEvent event = event_getter.get_next_event();	
			event.print();
			ListOfPropagations *photons = event.transform_to_mcTracer_photons();
			//photons->disp();
			photons->propagate(Mworld,&settings);
		}

		FreeOrbitCamera free(Mworld,&settings);

	}catch(std::exception &error){
		cerr << error.what(); 
	}	
	return 0;
}
