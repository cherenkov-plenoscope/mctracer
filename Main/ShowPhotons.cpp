#include "Core/Photons.h"
#include "Xml/Factory/SceneryFactory.h"
#include "PhotonSensor/PhotonSensor.h"
#include "Corsika/EventIo/EventIo.h"
#include "Corsika/Tools.h"
#include "Corsika/EventIo/PhotonFactory.h"
#include "Xml/Factory/VisualConfigFab.h"
#include "Xml/Factory/TracerSettingsFab.h"
#include "CommandLine/CommandLine.h"
#include "Cameras/FlyingCamera.h"
#include "Scenery/TrajectoryFactory.h"
using std::string;
using std::cout;

string help_text() {
	std::stringstream out; 
	out << "  Explore air shower event photons in a scenery in 1st person.\n";
	out << "  Objects, declared telescopes in the scenery, will be pointed\n";
	out << "  towards the source of the air shower event\n";
	out << "  --scenery, -s  path of scenery\n";
	out << "  --photons, -p  path of photons\n";
	out << "  --visual_config, -v  [optional]\n";
	return out.str();
}

int main(int argc, char* argv[]) {
	try{

	CommandLine::Parser cmd;
	cmd.define_key_val_by_key_short_desc("scenery", 's', "scenery path");
	cmd.define_key_val_by_key_short_desc("photons", 'p', "photon path");
	cmd.define_key_val_by_key_short_desc("visual_config", 'v', "visual config path");
	cmd.parse(argc, argv);

	if(
		!cmd.exist("scenery") || !cmd.exist("photons")
	) {
		cout << help_text();
		return 0;
	}

	// settings
	TracerSettings settings;
	settings.use_multithread_when_possible = false;
	settings.visual.photon_trajectories.radius = 2.0;

	if(!cmd.get("visual_config").empty()) {
		Xml::Document doc(cmd.get("visual_config"));
		Xml::Node node = doc.node();
		Xml::Node vc_node = node.child("visual");
		settings.visual = Xml::Configs::get_VisualConfig_from_node(vc_node);
	}
	
	// Random
	Random::Mt19937 prng(settings.pseudo_random_number_seed);

	// load scenery
	Frame world;
	world.set_name_pos_rot("root", Vec3::null, Rot3::null);
	Xml::SceneryFactory fab(cmd.get("scenery"));
	fab.add_scenery_to_frame(&world);
	world.init_tree_based_on_mother_child_relations();
	// init Telescope Array Control
	TelescopeArrayControl* array_ctrl = fab.telescopes;

	// load photons
	EventIo::Run corsika_run(cmd.get("photons"));

	// propagate each event
	uint event_counter = 0;

	FlyingCamera free_orb(&world, &settings);

	while(corsika_run.has_still_events_left()) {

		event_counter++;

		// read next event
		EventIo::Event event = corsika_run.next_event();

       	// point the telescope into shower direction
       	double az = Corsika::EventHeader::azimuth(event.header.raw);
       	double zd = Corsika::EventHeader::zenith(event.header.raw);
		array_ctrl->move_all_to_Az_Zd(az, zd);
		
        uint id = 0;
        for(array<float, 8> corsika_photon : event.photons) {
            
            vector<Photon*> photons;
            EventIo::PhotonFactory cpf(corsika_photon, id++, &prng);

            if(cpf.passed_atmosphere()) {
                photons.push_back(cpf.get_photon());
            
				// propagate the cherenkov photons in the world
				Photons::propagate_photons_in_scenery_with_settings(
					&photons, &world, &settings, &prng
				);

				for(Photon* ph: photons) {
					TrajectoryFactory traj(ph);
					traj.append_trajectory_to(&world);
					world.init_tree_based_on_mother_child_relations();
					free_orb.continue_with_new_scenery_and_settings(&world, &settings);
					traj.erase_trajectory_from(&world);
				}
				Photons::delete_photons(&photons);
			}
		}
	}

	}catch(std::exception &error) {
		std::cerr << error.what(); 
	}
	return 0;
}
