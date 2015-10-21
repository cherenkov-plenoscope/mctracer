#include "ToDoScheduler.h"
#include "PhotonBunch.h"
#include "ProgramOptions.h"
#include "XmlFactory/WorldFactory.h"
//------------------------------------------------------------------------------
ToDoScheduler::ToDoScheduler(int argc, char** argv) {

	define();
	comand_line_parser.parse_check(argc, argv);
}
//------------------------------------------------------------------------------
void ToDoScheduler::execute() {
	if (comand_line_parser.exist(render_key))
		render_geometry();
	else if (comand_line_parser.exist(propagate_key))
		propagate_photons_through_geometry();
	else if (comand_line_parser.exist(investigation_key))
		investigate_single_photon_propagation_in_geometry();
	else if (comand_line_parser.exist(pointsource_key))
		point_spread_investigation_in_geometry();
	else 
		std::cout << "Nothing to do, quit.\n --help for options" << std::endl;
}
//------------------------------------------------------------------------------
void ToDoScheduler::define() {

	comand_line_parser.add<std::string>(
		geometry_key, 'g', "geometry input file", false, ""
	);

	comand_line_parser.add<std::string>(
		photons_key, 'p', "photon input file",  false, ""
	);

	comand_line_parser.add<std::string>(
		config_key, 'c' ,"configuration file" , false, ""
	);

	comand_line_parser.add<std::string>(
		output_key, 'o', "output file", false, ""
	);

  	comand_line_parser.add(
  		pointsource_key, '\0', "point source investigation"
  	);	

    comand_line_parser.add(
  		render_key, '\0', "render geometry"
  	);	

    comand_line_parser.add(
  		propagate_key, '\0', "propagate photons through geometry"
  	);

    comand_line_parser.add(
  		investigation_key, '\0', "investigate single photon propagation in geometry"
  	);
}
//------------------------------------------------------------------------------
void ToDoScheduler::render_geometry()const {

	TracerSettings settings;	

	WorldFactory fab;
	fab.load(get_geometry_file());
	Frame *geometry = fab.world();
	FreeOrbitCamera free(geometry, &settings);
}
//------------------------------------------------------------------------------
void ToDoScheduler::point_spread_investigation_in_geometry()const {

	// Bokeh settings
	KeyValueMap source_config(get_config_file_name());

	// propagation settings
	TracerSettings settings;	
	settings.SetMultiThread(true);
	settings.SetStoreOnlyLastIntersection(true);

	// scenery
	WorldFactory fab;
	fab.load(get_geometry_file());
	Frame *world = fab.world();

	// sensors in scenery
	std::vector<PhotonSensor*>* sensors = fab.sensors_in_world();

	// photon source
	LightSourceFromConfig light_fab(source_config);
	std::vector<Photon*>* photon_bunch = light_fab.get_photons();

	// photon propagation
	PhotonBunch::propagate_photons_in_world_with_settings(
		photon_bunch, world, &settings
	);

	// detect photons in sensors
	PhotonSensors::reset_all_sesnors(sensors);
	PhotonSensors::assign_photons_to_sensors(photon_bunch, sensors);

	// write each sensors to file
	uint sensor_conuter = 0;
	for(PhotonSensor* sensor: *sensors) {
		
		std::stringstream outname;
		outname << get_output_file_name() << sensor_conuter;
		sensor_conuter++;

		AsciiIo::write_table_to_file(
			sensor->get_arrival_table_x_y_t(),
			outname.str()
		);
	}
}
//------------------------------------------------------------------------------
void ToDoScheduler::propagate_photons_through_geometry()const {

	// settings
	TracerSettings settings;	
	settings.SetMultiThread(false);
	settings.SetStoreOnlyLastIntersection(true);

	// load scenery
	WorldFactory fab;
	fab.load(get_geometry_file());
	Frame *world = fab.world();

	// init Telescope Array Control
	TelescopeArrayControl* array_ctrl = fab.get_telescope_array_control();

	// init sensors in scenery
	std::vector<PhotonSensor*>* sensors = fab.sensors_in_world();

	// load photons
	MmcsCorsikaFullEventGetter event_getter(get_photon_file_name());

	// propagate each event
	uint event_counter = 0;

	while(event_getter.has_still_events_left()) {

		MmcsCorsikaEvent event = event_getter.get_next_event();

		while(event.can_be_reused_again()) {

			event_counter++;

			// point the telescope into shower direction
			array_ctrl->move_all_to_Az_Zd(event.get_Az(), event.get_Zd());

			// get the cherenkov photons
			std::vector<Photon*> *photons = 
				event.use_once_more_and_get_photon_bunch();

			// propagate the cherenkov photons in the world
			PhotonBunch::propagate_photons_in_world_with_settings(
				photons, world, &settings
			);

			// detect photons in sensors
			PhotonSensors::reset_all_sesnors(sensors);
			PhotonSensors::assign_photons_to_sensors(photons, sensors);
			
			uint sensor_conuter = 0;
			for(PhotonSensor* sensor: *sensors) {
				
				std::stringstream outname;
				outname << get_output_file_name();
				outname << "e" << event_counter << "s" << sensor_conuter << ".txt";
				sensor_conuter++;

				AsciiIo::write_table_to_file(
					sensor->get_arrival_table_x_y_t(),
					outname.str()
				);
			}

			// wipe out the cherenkov photons which have just been propagated
			PhotonBunch::delete_photons_and_history(photons);
			std::cout << event_counter << "\n";
		}
	}
}
//------------------------------------------------------------------------------
void ToDoScheduler::investigate_single_photon_propagation_in_geometry()const {
	
	// init propagation settings
	TracerSettings settings;

	// load the scenery
	WorldFactory fab;
	fab.load(get_geometry_file());
	Frame *world = fab.world();

	// init Telescope Array Control
	TelescopeArrayControl* array_ctrl = fab.get_telescope_array_control();
	std::cout << array_ctrl->get_print();

	// load the photons
	MmcsCorsikaFullEventGetter event_getter(get_photon_file_name());

	// start interactive orbit, first without any photon trajectory
	FreeOrbitCamera free_orb(world, &settings);

	uint event_counter = 0;
	while(event_getter.has_still_events_left()) {

		MmcsCorsikaEvent event = event_getter.get_next_event();

		while(event.can_be_reused_again()) {

			event_counter++;

			// point the telescope into shower direction
			array_ctrl->move_all_to_Az_Zd(event.get_Az(), event.get_Zd());
			std::cout << array_ctrl->get_print();

			// get the cherenkov photons
			std::vector<Photon*> *photons = 
				event.use_once_more_and_get_photon_bunch();

			// propagate the cherenkov photons in the world
			PhotonBunch::propagate_photons_in_world_with_settings(
				photons, world, &settings
			);
			std::cout << "event_counter: " << event_counter << "\n";
			
			uint photon_counter = 0;
	
			PhotonBunch::Trajectories trayect_fab(photons);

			while(trayect_fab.has_still_trajectories_left() && photon_counter < 25) {

				photon_counter++;
				Frame SWorld = *world;
				SWorld.set_mother_and_child(trayect_fab.get_next_trajectoy());
				SWorld.init_tree_based_on_mother_child_relations();
				//std::cout << SWorld.get_tree_print();
				free_orb.continue_with_new_scenery_and_settings(&SWorld, &settings);
			}
			// wipe out the cherenkov photons which have just been propagated
			PhotonBunch::delete_photons_and_history(photons);
		}
	}
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::get_geometry_file()const {
	return comand_line_parser.get<std::string>(geometry_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::get_photon_file_name()const {
	return comand_line_parser.get<std::string>(photons_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::get_output_file_name()const {
	return comand_line_parser.get<std::string>(output_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::get_config_file_name()const {
	return comand_line_parser.get<std::string>(config_key);
}
