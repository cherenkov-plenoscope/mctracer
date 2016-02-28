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
#include "Xml/Factory/TracerSettingsFab.h"
#include "PhotonsReader/PhotonsReader.h"
//------------------------------------------------------------------------------
ToDoScheduler::ToDoScheduler(int argc, char** argv) {

	define_commands();
	cmd.parse(argc, argv);
	execute();
}
//------------------------------------------------------------------------------
void ToDoScheduler::execute() {

	if (cmd.exist("show"))
		show_scenery();
	else if (cmd.exist("propagate"))
		propagate();
	else if (cmd.exist("investigate"))
		investigate_events();
	else 
		cout << "No command found, quit\n\n" << help();
}
//------------------------------------------------------------------------------
string ToDoScheduler::help()const {
	std::stringstream out; 
	out << help_show_scenery() << "\n";
	out << help_propagate() << "\n";
	out << help_investigate_events() << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
void ToDoScheduler::define_commands() {

	cmd.define_key_val_by_key_short_desc("scenery", 's', "scenery path");
	cmd.define_key_val_by_key_short_desc("photons", 'p', "photon path");
	cmd.define_key_val_by_key_short_desc("config", 'c' ,"configuration path");
	cmd.define_key_val_by_key_short_desc("output", 'o', "output path");
    cmd.define_command("show", "show scenery");	
    cmd.define_command("propagate", "propagate photons through scenery");
    cmd.define_command("investigate", "investigate single photon propagation in scenery");
}
//------------------------------------------------------------------------------
const string ToDoScheduler::scenery_path()const {
	return cmd.get("scenery");
}
//------------------------------------------------------------------------------
const string ToDoScheduler::photon_path()const {
	return cmd.get("photons");
}
//------------------------------------------------------------------------------
const string ToDoScheduler::output_path()const {
	return cmd.get("output");
}
//------------------------------------------------------------------------------
const string ToDoScheduler::config_path()const {
	return cmd.get("config");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool ToDoScheduler::command_is_complete_show_scenery()const {
	if(!cmd.exist("scenery")) {
		cout << help_show_scenery();
		return false;
	}else
		return true;
}
//------------------------------------------------------------------------------
void ToDoScheduler::show_scenery()const {

	if(!command_is_complete_show_scenery())
		return;

	Frame *scenery;
	if(
		StringTools::is_ending(scenery_path(),".stl") || 
		StringTools::is_ending(scenery_path(),".STL")
	) {
		scenery = StereoLitographyIo::read(scenery_path(), 1.0);
	}else if(	
		StringTools::is_ending(scenery_path(),".xml") ||
		StringTools::is_ending(scenery_path(),".XML")
	) {
		WorldFactory fab;
		fab.load(scenery_path());
		scenery = fab.world();
	}else{
		scenery = Frame::void_frame;
		cout << "Can only read stl or xml files.\n";
		return;
	}
	
	TracerSettings settings;

	if(!config_path().empty()) {
		Xml::Document doc(config_path());
		Xml::Node node = doc.get_tree();
		Xml::Node vc_node = node.child("visual");
		settings.visual = Xml::Configs::get_VisualConfig_from_node(vc_node);
	}

	FlyingCamera free(scenery, &settings);
}
//------------------------------------------------------------------------------
string ToDoScheduler::help_show_scenery()const {
	std::stringstream out; 
	out << "--show\n";
	out << "  Explore a scenery in 1st person view\n";
	out << "  --scenery, -s  path of scenery\n";
	out << "  --config,  -c  [optional]\n";
	return out.str();	
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool ToDoScheduler::command_is_complete_propagate()const {
	if(
		!cmd.exist("scenery") ||
		!cmd.exist("photons") ||
		!cmd.exist("config") ||
		!cmd.exist("output")
	) {
		cout << help_propagate();
		return false;
	}else
		return true;
}
//------------------------------------------------------------------------------
void ToDoScheduler::propagate()const {

	if(!command_is_complete_propagate())
		return;

	// Bokeh settings
	Xml::Document doc(config_path());
	Xml::Node se = doc.get_tree().child("settings");
   
    //--------------------------------------------------------------------------
    // BASIC SETTINGS
	TracerSettings settings = Xml::Configs::get_TracerSettings_from_node(se);
	
	//--------------------------------------------------------------------------
	// Random
	Random::Mt19937 prng(settings.pseudo_random_number_seed);
	
	//--------------------------------------------------------------------------
	// scenery
	WorldFactory fab;
	fab.load(scenery_path());
	Frame *world = fab.world();
	
	//--------------------------------------------------------------------------
	// sensors in scenery
	PhotonSensors::Sensors sensors = fab.get_sensors();
	
	//--------------------------------------------------------------------------
	// photon source
	PhotonsReader photon_file(photon_path());
	std::vector<Photon*>* photons;

	uint event_counter = 1;
	while(photon_file.has_still_photons_left()) {
		photons = photon_file.next(&prng);

		//----------------------------------------------------------------------
		// photon propagation
		Photons::propagate_photons_in_world_with_settings(
			photons, world, &settings, &prng
		);

		//----------------------------------------------------------------------
		// detect photons in sensors
		sensors.clear_history();
		sensors.assign_photons(photons);
		//----------------------------------------------------------------------
		// write each sensors to file
		for(uint i=0; i<sensors.size(); i++) {

			std::stringstream outname;
			outname << output_path() << event_counter << "_" << i;

			std::stringstream header;
			header << "scenery: " << scenery_path() << "\n";
			header << "sensor:  " << sensors.at(i)->get_frame()->get_path_in_tree_of_frames() << ", ID: " << i << "\n";
			header << "photons: " << photon_path() << "\n";
			header << "-------------\n";
			header << sensors.at(i)->get_arrival_table_header();

			AsciiIo::write_table_to_file_with_header(
				sensors.at(i)->get_arrival_table(),
				outname.str(),
				header.str()
			);
		}
		//----------------------------------------------------------------------
		Photons::delete_photons_and_history(photons);
		delete photons;
		event_counter++;
	}
}
//------------------------------------------------------------------------------
string ToDoScheduler::help_propagate()const {
	std::stringstream out; 
	out << "--propagate\n";
	out << "  Propagate photons in scenery.\n";
	out << "  --scenery, -s  path of scenery\n";
	out << "  --photons, -p  path of photons\n";
	out << "  --config,  -c  path of mctracer configuration\n";
	out << "  --output,  -o  path to write \n";
	return out.str();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/*void ToDoScheduler::propagate_photons_through_scenery()const {

	// settings
	TracerSettings settings;	
	settings.use_multithread_when_possible = false;
	settings.store_only_final_intersection = true;

	// Random
	Random::Mt19937 prng(settings.pseudo_random_number_seed);

	// load scenery
	WorldFactory fab;
	fab.load(scenery_path());
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
			header << "scenery: " << scenery_path() << "\n";
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
}*/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool ToDoScheduler::command_is_complete_investigate_events()const {
	if(
		!cmd.exist("scenery") ||
		!cmd.exist("photons") ||
		!cmd.exist("config")
	) {
		cout << help_investigate_events();
		return false;
	}else
		return true;
}
//------------------------------------------------------------------------------
void ToDoScheduler::investigate_events()const {

	if(!command_is_complete_investigate_events())
		return;

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
	fab.load(scenery_path());
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
					//cout << SWorld.get_tree_print();
					free_orb.continue_with_new_scenery_and_settings(&SWorld, &settings);
				}

				Photons::delete_photons_and_history(&photons);
			}
		}
	}
}
//------------------------------------------------------------------------------
string ToDoScheduler::help_investigate_events()const {
	std::stringstream out; 
	out << "--investigate\n";
	out << "  Explore air shower event photons in a scenery in 1st person.\n";
	out << "  Objects, declared telescopes in the scenery, will be pointed\n";
	out << "  towards the source of the air shower event\n";
	out << "  --scenery, -s  path of scenery\n";
	out << "  --photons, -p  path of photons\n";
	out << "  --config,  -c  path of mctracer configuration\n";
	return out.str();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////