#include "ToDoScheduler.h"
#include "Core/Photons.h"
#include "Tools/AsciiIo.h"
#include "KeyValueMap.h"
#include "XmlFactory/WorldFactory.h"
#include "Geometry/StereoLitographyIo.h"
#include "PhotonSensor/PhotonSensor.h"
#include "MmcsCorsikaFileIO/EventIo.h"
#include "MmcsCorsikaFileIO/CorsikaPhotonFactory.h"
//------------------------------------------------------------------------------
ToDoScheduler::ToDoScheduler(int argc, char** argv) {

	define();
	comand_line_parser.parse_check(argc, argv);
	execute();
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

	comand_line_parser.add<std::string>(
		skydome_key, 's', "sky dome image", false, ""
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

	Frame *geometry;
	if(
		StringTools::is_ending(geometry_path(),".stl") || 
		StringTools::is_ending(geometry_path(),".STL")
	) {
		geometry = StereoLitographyIo::read(geometry_path(), 1.0);
	}else if(	StringTools::is_ending(geometry_path(),".xml") ||
				StringTools::is_ending(geometry_path(),".XML")
	) {
		WorldFactory fab;
		fab.load(geometry_path());
		geometry = fab.world();
	}else{
		geometry = Frame::void_frame;
		std::cout << "Can only read stl or xml files.\n";
		return;
	}
	
	TracerSettings settings;

	if(!skydome_path().empty()) {
		SkyDome::Texture *sky = new SkyDome::Texture(skydome_path());
		settings.set_sky_dome(sky);
		FreeOrbitCamera free(geometry, &settings);
	}else{
		FreeOrbitCamera free(geometry, &settings);
	}
}
//------------------------------------------------------------------------------
void ToDoScheduler::point_spread_investigation_in_geometry()const {

	// Bokeh settings
	KeyValueMap::Map source_config(config_path());

	// propagation settings
	TracerSettings settings;	
	settings.SetMultiThread(true);
	settings.store_only_last_intersection(true);

	// scenery
	WorldFactory fab;
	fab.load(geometry_path());
	Frame *world = fab.world();
	// sensors in scenery
	PhotonSensors::Sensors sensors = fab.get_sensors();

	// photon source
	LightSourceFromConfig light_fab(source_config);
	std::vector<Photon*>* photons = light_fab.get_photons();

	// photon propagation
	Photons::propagate_photons_in_world_with_settings(
		photons, world, &settings
	);

	// detect photons in sensors
	sensors.clear_history();
	sensors.assign_photons(photons);

	// write each sensors to file
	for(uint i=0; i<sensors.size(); i++) {

		std::stringstream outname;
		outname << output_path() << i;

		std::stringstream header;
		header << "geometry: " << geometry_path() << "\n";
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

	Random::Mt19937 prng(Random::zero_seed);

	// settings
	TracerSettings settings;	
	settings.SetMultiThread(false);
	settings.store_only_last_intersection(true);

	// load scenery
	WorldFactory fab;
	fab.load(geometry_path());
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
            
            CorsikaPhotonFactory cpf(corsika_photon, id++, &prng);

            if(cpf.passed_atmosphere())
                photons.push_back(cpf.get_photon());
        }

       	// point the telescope into shower direction
       	double az = event.header.mmcs_event_header.azimuth_angle_Phi_in_radian;
       	double zd = event.header.mmcs_event_header.zenith_angle_Theta_in_radian;
		array_ctrl->move_all_to_Az_Zd(az, zd);

		// propagate the cherenkov photons in the world
		Photons::propagate_photons_in_world_with_settings(
			&photons, world, &settings
		);

		// detect photons in sensors
		sensors.clear_history();
		sensors.assign_photons(&photons);

		for(uint i=0; i<sensors.size(); i++) {
			
			std::stringstream outname;
			outname << output_path();
			outname << "e" << event_counter << "s" << sensors.at(i)->get_id() << ".txt";

			std::stringstream header;
			header << "geometry: " << geometry_path() << "\n";
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
/*
void ToDoScheduler::propagate_photons_through_geometry()const {

	// settings
	TracerSettings settings;	
	settings.SetMultiThread(false);
	settings.store_only_last_intersection(true);

	// load scenery
	WorldFactory fab;
	fab.load(geometry_path());
	Frame *world = fab.world();

	// init Telescope Array Control
	TelescopeArrayControl* array_ctrl = fab.get_telescope_array_control();

	// init sensors in scenery
	std::vector<PhotonSensor::Sensor*>* sensors = fab.sensors_in_world();

	// load photons
	MmcsCorsikaFullEventGetter event_getter(photon_path());

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
			Photons::propagate_photons_in_world_with_settings(
				photons, world, &settings
			);

			// detect photons in sensors
			PhotonSensor::Sensors::reset_all_sesnors(sensors);
			PhotonSensor::Sensors::assign_photons_to_sensors(photons, sensors);
			
			uint sensor_conuter = 0;
			for(PhotonSensor::Sensor* sensor: *sensors) {
				
				std::stringstream outname;
				outname << output_path();
				outname << "e" << event_counter << "s" << sensor_conuter << ".txt";
				sensor_conuter++;

				AsciiIo::write_table_to_file(
					sensor->get_arrival_table(),
					outname.str()
				);
			}

			// wipe out the cherenkov photons which have just been propagated
			Photons::delete_photons_and_history(photons);
			std::cout << event_counter << "\n";
		}
	}
}*/
//------------------------------------------------------------------------------
void ToDoScheduler::investigate_single_photon_propagation_in_geometry()const {


	Random::Mt19937 prng(Random::zero_seed);

	// settings
	TracerSettings settings;	
	settings.SetMultiThread(false);
	settings.store_only_last_intersection(true);
	settings.trajectory_radius = 2.0;

	// load scenery
	WorldFactory fab;
	fab.load(geometry_path());
	Frame *world = fab.world();

	// init Telescope Array Control
	TelescopeArrayControl* array_ctrl = fab.get_telescope_array_control();

	// load photons
	EventIo::EventIoFile corsika_run(photon_path());

	// propagate each event
	uint event_counter = 0;

	FreeOrbitCamera free_orb(world, &settings);

	while(corsika_run.has_still_events_left()) {

		event_counter++;

		// read next evenr
		EventIo::Event event = corsika_run.next_event();

		vector<Photon*> photons;
        uint id = 0;
        for(vector<float> corsika_photon : event.photons) {
            
            CorsikaPhotonFactory cpf(corsika_photon, id++, &prng);

            if(cpf.passed_atmosphere())
                photons.push_back(cpf.get_photon());
        }

       	// point the telescope into shower direction
       	double az = event.header.mmcs_event_header.azimuth_angle_Phi_in_radian;
       	double zd = event.header.mmcs_event_header.zenith_angle_Theta_in_radian;
		array_ctrl->move_all_to_Az_Zd(az, zd);

		// propagate the cherenkov photons in the world
		Photons::propagate_photons_in_world_with_settings(
			&photons, world, &settings
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
//------------------------------------------------------------------------------
const std::string ToDoScheduler::geometry_path()const {
	return comand_line_parser.get<std::string>(geometry_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::photon_path()const {
	return comand_line_parser.get<std::string>(photons_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::output_path()const {
	return comand_line_parser.get<std::string>(output_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::config_path()const {
	return comand_line_parser.get<std::string>(config_key);
}
//------------------------------------------------------------------------------
const std::string ToDoScheduler::skydome_path()const {
	return comand_line_parser.get<std::string>(skydome_key);
}
