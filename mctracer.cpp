#include "WorldFactory.h"
#include "Tools/Tools.h"
#include "Core/Photon.h"
#include "Core/ListOfPropagations.h"
#include "Cameras/FreeOrbitCamera.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaFullEventGetter.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaPhotonData.h"
#include "TelescopeFrame.h"
#include "SegmetedReflectorGenerator.h"
#include "EllipticalCapWithHexagonalBound.h"
#include "Histogramm.h"
#include "ProgramOptions.h"

int main(const int argc, const char* argv[]) {
	try{
		UserInteraction::print_welcome_screen();

		KeyValueMap options(argc, argv);
		//std::cout << options;

		TracerSettings settings;		
		
		std::string file_chosen_by_user = 
			UserInteraction::input("Enter a world file to load: ");

		WorldFactory file2world;
		file2world.load(file_chosen_by_user);
		Frame *Eorld = file2world.world();

		FreeOrbitCamera free(Eorld, &settings);
		
		//TELESCOPE world
		Frame world;
		world.set_name_pos_rot("world", Vector3D::null, Rotation3D::null);

		//ground
		Disc ground;
		ground.set_name_pos_rot("ground", Vector3D(0.0, 0.0, -2.0), Rotation3D::null);
 		
		Color* ground_color = new Color(0,55,0);
		ground.set_disc_radius(1e3);
 		ground.set_outer_color(ground_color);
 		ground.set_inner_color(ground_color);

		//telescope
		TelescopeFrame FACT;
		FACT.set_name_pos_rot("FACT", Vector3D::null, Rotation3D::null);

		//reflector
		SegmetedReflectorGenerator reflector_generator;

		reflector_generator.set_focal_length(4.889);
		reflector_generator.set_facet_spacing(0.62);
		reflector_generator.set_max_outer_diameter(3.5);
		reflector_generator.set_min_inner_diameter(0.5);
		reflector_generator.set_hybrid_geometry(0.5);
		Frame* reflector = reflector_generator.get_reflector();
		//std::cout << reflector_generator.get_print();

		//sensor
		Disc Sensor;
		Sensor.set_name_pos_rot("Sensor", Vector3D(0.0, 0.0, 4.889), Rotation3D::null);
 		
		Color* sensor_color = new Color(255,0,0);
		Sensor.set_disc_radius(0.32);//(0.185);
 		Sensor.set_outer_color(sensor_color);
 		Sensor.set_inner_color(sensor_color);

		/*EllipticalCapWithHexagonalBound gg;
		gg.set_name_pos_rot("ellpis", Vector3D::unit_x*10.0, Rotation3D::null);
		gg.set_focalLength_radiiRatio_hexAngel_hexRadius(
			4.9, 1.5, 0.0, 0.31  
		);
		gg.set_outer_color(sensor_color);
 		gg.set_inner_color(sensor_color);*/

		//sensor_housing_lower_cap
		Disc sensor_housing_lower_cap;
		sensor_housing_lower_cap.set_name_pos_rot("sensor_housing_lower_cap", Vector3D(0.0, 0.0, 4.890), Rotation3D::null);
		Color* sensor_housing_color = new Color(200,255,200);
		sensor_housing_lower_cap.set_disc_radius(0.32);
 		sensor_housing_lower_cap.set_outer_color(sensor_housing_color);
 		sensor_housing_lower_cap.set_inner_color(sensor_housing_color);

 		//relations
		FACT.set_mother_and_child(reflector);
		FACT.set_mother_and_child(&Sensor);
		FACT.set_mother_and_child(&sensor_housing_lower_cap);	

		world.set_mother_and_child(&FACT);
		world.set_mother_and_child(&ground);
		//world.set_mother_and_child(&gg);
		world.init_tree_based_on_mother_child_relations();
		
		// CORSIKA PHOTONS
		//string filename = UserInteraction::input("Enter Mmcs CORSIKA file: ");
		std::string filename = "../../cer002600";
		//std::string filename = "../../cer000002";
		MmcsCorsikaFullEventGetter event_getter(filename);


		Histogramm hist(0, 1000e-9, 1000);
		uint event_counter = 0;
		while(event_getter.has_still_events_left()) {

			MmcsCorsikaEvent event = event_getter.get_next_event();	

			//std::cout << event.get_print() << std::endl;
			while(event.can_be_reused_again()) {

				event_counter++;

				FACT.move_to_Az_Zd(event.get_Az(), event.get_Zd());

				ListOfPropagations *photons = 
					event.use_once_more_and_get_photons();
			
				photons->propagate_in_world_with_settings(&world, &settings);

				std::cout << "event_counter: " << event_counter << "\n";
				hist.fill_in(photons->get_wavelength_vector());
				
				//std::cout << event.get_print() << "\n";
				//std::cout << FACT.get_pointing_print() << "\n";

				if(event_counter == 1) {

					/*std::cout << photons->get_csv_print_for_propagations_ending_in(
						world.get_frame_in_tree_by_path(
							"FACT/Sensor"
						)
					);*/
					
					uint count = 0;
					
					while(photons->has_still_trajectoies_left() && count < 5) {

						count++;
						Frame SWorld = world;
						SWorld.set_mother_and_child(photons->get_next_trajectoy());
						SWorld.init_tree_based_on_mother_child_relations();
						std::cout << SWorld.get_tree_print();

						FreeOrbitCamera free(&SWorld, &settings);
					}
					
				}
				delete photons;
			}
		}

		FileTools::write_text_to_file(hist.get_print(), "wavelengths_watz.csv");

	}catch(std::exception &error){
		std::cerr << error.what(); 
	}	
	return 0;
}