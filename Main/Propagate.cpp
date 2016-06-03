#include "Core/Photons.h"
#include "Tools/AsciiIo.h"
#include "Xml/Factory/SceneryFactory.h"
#include "PhotonSensor/PhotonSensor.h"
#include "Xml/Factory/TracerSettingsFab.h"
#include "PhotonsReader/PhotonsReader.h"
#include "CommandLine/CommandLine.h"
using std::string;
using std::cout;

string help_text() {
	std::stringstream out; 
	out << "  Propagate photons in scenery.\n";
	out << "  --scenery, -s  path of scenery\n";
	out << "  --photons, -p  path of photons\n";
	out << "  --config,  -c  path of mctracer configuration\n";
	out << "  --output,  -o  path to write \n";
	return out.str();
}

int main(int argc, char* argv[]) {
	try{
	//--------------------------------------------------------------------------

	CommandLine::Parser cmd;
	cmd.define_key_val_by_key_short_desc("scenery", 's', "scenery path");
	cmd.define_key_val_by_key_short_desc("photons", 'p', "photon path");
	cmd.define_key_val_by_key_short_desc("config", 'c' ,"configuration path");
	cmd.define_key_val_by_key_short_desc("output", 'o', "output path");
	cmd.parse(argc, argv);

	if(
		!cmd.exist("scenery") || !cmd.exist("photons") || 
		!cmd.exist("config") ||	!cmd.exist("output")
	) {
		cout << help_text();
		return 0;
	}

	// Bokeh settings
	Xml::Document doc(cmd.get("config"));
	Xml::Node se = doc.node().child("settings");
   
    //--------------------------------------------------------------------------
    // BASIC SETTINGS
	TracerSettings settings = Xml::Configs::get_TracerSettings_from_node(se);
	
	//--------------------------------------------------------------------------
	// Random
	Random::Mt19937 prng(settings.pseudo_random_number_seed);
	
	//--------------------------------------------------------------------------
	// scenery
	Frame *world = new Frame("root", Vec3::null, Rot3::null);
	Xml::SceneryFactory fab(cmd.get("scenery"));
	fab.add_scenery_to_frame(world);
	world->init_tree_based_on_mother_child_relations();
	//--------------------------------------------------------------------------
	// sensors in scenery
	PhotonSensors::Sensors sensors = fab.sensors();
	
	//--------------------------------------------------------------------------
	// photon source
	PhotonsReader photon_file(cmd.get("photons"));
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
			outname << cmd.get("output") << event_counter << "_" << i;

			std::stringstream header;
			header << "scenery: " << cmd.get("scenery") << "\n";
			header << "sensor:  " << sensors.at(i)->get_frame()->get_path_in_tree_of_frames() << ", ID: " << i << "\n";
			header << "photons: " << cmd.get("photons") << "\n";
			header << "-------------\n";
			header << sensors.at(i)->get_arrival_table_header();

			AsciiIo::write_table_to_file_with_header(
				sensors.at(i)->get_arrival_table(),
				outname.str(),
				header.str()
			);
		}
		//----------------------------------------------------------------------
		Photons::delete_photons(photons);
		delete photons;
		event_counter++;
	}
	//--------------------------------------------------------------------------
	}catch(std::exception &error) {
		std::cerr << error.what(); 
	}
	return 0;
}