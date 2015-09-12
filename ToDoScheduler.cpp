#include "ToDoScheduler.h"
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
		"conf", 'c' ,"configuration file" , false, ""
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
void ToDoScheduler::propagate_photons_through_geometry()const {

}
//------------------------------------------------------------------------------
void ToDoScheduler::investigate_single_photon_propagation_in_geometry()const {
	
	// init propagation settings
	TracerSettings settings;

	// load the scenery
	WorldFactory fab;
	fab.load(get_geometry_file());
	Frame *world = fab.world();

	// load the photons
	MmcsCorsikaFullEventGetter event_getter(get_photon_file());

	// start interactive orbit, first without any photon trajectory
	FreeOrbitCamera free_orb(world, &settings);

	uint event_counter = 0;
	while(event_getter.has_still_events_left()) {

		MmcsCorsikaEvent event = event_getter.get_next_event();

		while(event.can_be_reused_again()) {

			event_counter++;
			world->move_all_telescopes_to_Az_Zd(event.get_Az(), event.get_Zd());
			ListOfPropagations *photons = event.use_once_more_and_get_photons();
			photons->propagate_in_world_with_settings(world, &settings);
			std::cout << "event_counter: " << event_counter << "\n";
			
			uint photon_counter = 0;
	
			while(photons->has_still_trajectoies_left() && photon_counter < 25) {

				photon_counter++;
				Frame SWorld = *world;
				SWorld.set_mother_and_child(photons->get_next_trajectoy());
				SWorld.init_tree_based_on_mother_child_relations();
				std::cout << SWorld.get_tree_print();
				free_orb.continue_with_new_scenery_and_settings(&SWorld, &settings);
			}
			delete photons;
		}
	}
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::get_geometry_file()const {
	return comand_line_parser.get<std::string>(geometry_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::get_photon_file()const {
	return comand_line_parser.get<std::string>(photons_key);
}