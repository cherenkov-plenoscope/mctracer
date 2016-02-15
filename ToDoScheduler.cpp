#include "ToDoScheduler.h"
#include "Core/Photons.h"
#include "Tools/AsciiIo.h"
#include "KeyValueMap.h"
#include "XmlFactory/WorldFactory.h"
#include "Geometry/StereoLitographyIo/StereoLitographyIo.h"
#include "PhotonSensor/PhotonSensor.h"
#include "CorsikaIO/EventIo/EventIo.h"
#include "CorsikaIO/EventIo/PhotonFactory.h"
#include "Xml/Factory/VisualConfigFab.h"
//------------------------------------------------------------------------------
ToDoScheduler::ToDoScheduler(int argc, char** argv) {

	define();
	cmd.parse(argc, argv);
	execute();
}
//------------------------------------------------------------------------------
void ToDoScheduler::execute() {
	if (cmd.exist(render_key))
		render_geometry();
	else if (cmd.exist(propagate_key))
		propagate_photons_through_geometry();
	else if (cmd.exist(investigation_key))
		investigate_single_photon_propagation_in_geometry();
	else if (cmd.exist(pointsource_key))
		point_spread_investigation_in_geometry();
	else 
		std::cout << "Nothing to do, quit.\n --help for options" << std::endl;
}
//------------------------------------------------------------------------------
void ToDoScheduler::define() {

	cmd.add_value(geometry_key, 'g', "geometry input file");
	cmd.add_value(photons_key, 'p', "photon input file");
	cmd.add_value(config_key, 'c' ,"configuration file");
	cmd.add_value(output_key, 'o', "output file");
	cmd.add_value(skydome_key, 's', "sky dome image");
  	cmd.add_flag(pointsource_key, "point source investigation");	
    cmd.add_flag(render_key, "render geometry");	
    cmd.add_flag(propagate_key, "propagate photons through geometry");
    cmd.add_flag(investigation_key, "investigate single photon propagation in geometry");
}
//------------------------------------------------------------------------------
void ToDoScheduler::render_geometry()const {

	std::string geom_path = cmd.get(geometry_key);

	Frame *geometry;
	if(
		StringTools::is_ending(geom_path,".stl") || 
		StringTools::is_ending(geom_path,".STL")
	) {
		geometry = StereoLitographyIo::read(geom_path, 1.0);
	}else if(	StringTools::is_ending(geom_path,".xml") ||
				StringTools::is_ending(geom_path,".XML")
	) {
		WorldFactory fab;
		fab.load(geom_path);
		geometry = fab.world();
	}else{
		geometry = Frame::void_frame;
		std::cout << "Can only read stl or xml files.\n";
		return;
	}
	
	TracerSettings settings;

	if(!config_path().empty()) {
		Xml::Document doc(config_path());
		Xml::Node node = doc.get_tree();
		Xml::Node vc_node = node.child("visual");
		settings.visual = Xml::Configs::get_VisualConfig_from_node(vc_node);
	}

	FlyingCamera free(geometry, &settings);
}
//------------------------------------------------------------------------------
void ToDoScheduler::point_spread_investigation_in_geometry()const {

	// Bokeh settings
	KeyValueMap::Map source_config(config_path());

	// propagation settings
	TracerSettings settings;	
	settings.use_multithread_when_possible = true;
	settings.store_only_final_intersection = true;

	// Random
	Random::Mt19937 prng(settings.pseudo_random_number_seed);

	// scenery
	WorldFactory fab;
	fab.load(cmd.get(geometry_key));
	Frame *world = fab.world();
	// sensors in scenery
	PhotonSensors::Sensors sensors = fab.get_sensors();

	// photon source
	LightSourceFromConfig light_fab(source_config);
	std::vector<Photon*>* photons = light_fab.get_photons();

	// photon propagation
	Photons::propagate_photons_in_world_with_settings(
		photons, world, &settings, &prng
	);

	// detect photons in sensors
	sensors.clear_history();
	sensors.assign_photons(photons);

	// write each sensors to file
	for(uint i=0; i<sensors.size(); i++) {

		std::stringstream outname;
		outname << output_path() << i;

		std::stringstream header;
		header << "geometry: " << cmd.get(geometry_key) << "\n";
		header << "lightsource: " << config_path() << "\n";
		header << "sensor id: " << i << "\n";
		header << "sensor name: " << sensors.at(i)->get_frame()->get_path_in_tree_of_frames() << "\n";
		header << source_config.get_print();
		header << "-------------\n";
		header << sensors.at(i)->get_arrival_table_header();

		AsciiIo::write_table_to_file_with_header(
			sensors.at(i)->get_arrival_table(),
			outname.str(),
			header.str()
		);
	}

	Photons::delete_photons_and_history(photons);
}
//------------------------------------------------------------------------------
void ToDoScheduler::propagate_photons_through_geometry()const {

	// settings
	TracerSettings settings;	
	settings.use_multithread_when_possible = false;
	settings.store_only_final_intersection = true;

	// Random
	Random::Mt19937 prng(settings.pseudo_random_number_seed);

	// load scenery
	WorldFactory fab;
	fab.load(cmd.get(geometry_key));
	Frame *world = fab.world();
	PhotonSensors::Sensors sensors = fab.get_sensors();

	// init Telescope Array Control
	TelescopeArrayControl* array_ctrl = fab.get_telescope_array_control();

	// load photons
	EventIo::EventIoFile corsika_run(photon_path());

	// propagate each event
	uint event_counter = 0;

	while(corsika_run.has_still_events_left()) {

		event_counter++;

		// read next evenr
		EventIo::Event event = corsika_run.next_event();

		vector<Photon*> photons;
        uint id = 0;
        for(vector<float> corsika_photon : event.photons) {
            
            EventIo::PhotonFactory cpf(corsika_photon, id++, &prng);

            if(cpf.passed_atmosphere())
                photons.push_back(cpf.get_photon());
        }

       	// point the telescope into shower direction
       	double az = event.header.mmcs_event_header.azimuth_angle_Phi_in_radian;
       	double zd = event.header.mmcs_event_header.zenith_angle_Theta_in_radian;
		array_ctrl->move_all_to_Az_Zd(az, zd);

		// propagate the cherenkov photons in the world
		Photons::propagate_photons_in_world_with_settings(
			&photons, world, &settings, &prng
		);

		// detect photons in sensors
		sensors.clear_history();
		sensors.assign_photons(&photons);

		for(uint i=0; i<sensors.size(); i++) {
			
			std::stringstream outname;
			outname << output_path();
			outname << "e" << event_counter << "s" << sensors.at(i)->get_id() << ".txt";

			std::stringstream header;
			header << "geometry: " << cmd.get(geometry_key) << "\n";
			header << "photons: " << photon_path() << ", event: " << event_counter << "\n";
			header << "sensor id: " << i << "\n";
			header << "sensor name: " << sensors.at(i)->get_frame()->get_path_in_tree_of_frames() << "\n";
			header << "-------------\n";
			header << event.header.mmcs_event_header.get_print() << "\n";
			header << "-------------\n";
			header << sensors.at(i)->get_arrival_table_header();

			AsciiIo::write_table_to_file_with_header(
				sensors.at(i)->get_arrival_table(),
				outname.str(),
				header.str()
			);
		}

		// wipe out the cherenkov photons which have just been propagated
		Photons::delete_photons_and_history(&photons);
	}
}
//------------------------------------------------------------------------------
void ToDoScheduler::investigate_single_photon_propagation_in_geometry()const {

	// settings
	TracerSettings settings;
	settings.use_multithread_when_possible = false;
	settings.store_only_final_intersection = false;
	settings.visual.photon_trajectories.radius = 2.0;

	if(!config_path().empty()) {
		Xml::Document doc(config_path());
		Xml::Node node = doc.get_tree();
		Xml::Node vc_node = node.child("visual");
		settings.visual = Xml::Configs::get_VisualConfig_from_node(vc_node);
	}
	
	// Random
	Random::Mt19937 prng(settings.pseudo_random_number_seed);

	// load scenery
	WorldFactory fab;
	fab.load(cmd.get(geometry_key));
	Frame *world = fab.world();

	// init Telescope Array Control
	TelescopeArrayControl* array_ctrl = fab.get_telescope_array_control();

	// load photons
	EventIo::EventIoFile corsika_run(photon_path());

	// propagate each event
	uint event_counter = 0;

	FlyingCamera free_orb(world, &settings);

	while(corsika_run.has_still_events_left()) {

		event_counter++;

		// read next event
		EventIo::Event event = corsika_run.next_event();

       	// point the telescope into shower direction
       	double az = event.header.mmcs_event_header.azimuth_angle_Phi_in_radian;
       	double zd = event.header.mmcs_event_header.zenith_angle_Theta_in_radian;
		array_ctrl->move_all_to_Az_Zd(az, zd);
		
        uint id = 0;
        for(vector<float> corsika_photon : event.photons) {
            
            vector<Photon*> photons;
            EventIo::PhotonFactory cpf(corsika_photon, id++, &prng);

            if(cpf.passed_atmosphere()) {
                photons.push_back(cpf.get_photon());
            
				// propagate the cherenkov photons in the world
				Photons::propagate_photons_in_world_with_settings(
					&photons, world, &settings, &prng
				);

				Photons::Trajectories trayect_fab(&photons, &settings);

				while(trayect_fab.has_still_trajectories_left()) {

					Frame SWorld = *world;
					SWorld.set_mother_and_child(trayect_fab.get_next_trajectoy());
					SWorld.init_tree_based_on_mother_child_relations();
					//std::cout << SWorld.get_tree_print();
					free_orb.continue_with_new_scenery_and_settings(&SWorld, &settings);
				}

				Photons::delete_photons_and_history(&photons);
			}
		}
	}
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::geometry_path()const {
	return cmd.get(geometry_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::photon_path()const {
	return cmd.get(photons_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::output_path()const {
	return cmd.get(output_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::config_path()const {
	return cmd.get(config_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::skydome_path()const {
	return cmd.get(skydome_key);
}
