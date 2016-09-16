#include "DocOpt/docopt.h"
#include "Core/Photons.h"
#include "Xml/Factory/SceneryFactory.h"
#include "PhotonSensor/PhotonSensor.h"
#include "Corsika/EventIo/EventIo.h"
#include "Corsika/Tools.h"
#include "Corsika/EventIo/PhotonFactory.h"
#include "Xml/Factory/VisualConfigFab.h"
#include "Xml/Factory/TracerSettingsFab.h"
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

static const char USAGE[] =
R"(Show a scenery with photons

    Usage:
      mctShow --scenery=SCENERY_PATH --input=PHOTON_PATH [--config=CONFIG_PATH]
      mctShow (-h | --help)
      mctShow --version

    Options:
      -s --scenery=SCENERY_PATH     Scenery xml file path.
      -i --input=INPUT_PATH         Photon file path (e.g. a CORSIKA run).
      -c --config=CONFIG_PATH       Visual config xml file path
      -h --help                     Show this screen.
      --version                     Show version.
       
)";

int main(int argc, char* argv[]) {
    try{

    std::map<std::string, docopt::value> args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true,        // show help if requested
        "mct 0.0"
    );  // version string

    PathTools::Path scenery_path = PathTools::Path(args.find("--scenery")->second.asString());
    PathTools::Path photon_path = PathTools::Path(args.find("--input")->second.asString());
    
    // settings
    TracerSettings settings;
    settings.use_multithread_when_possible = false;
    settings.visual.photon_trajectories.radius = 2.0;

    if(args.find("--config")->second) {
        Xml::Document doc(args.find("--config")->second.asString());
        Xml::Node node = doc.node();
        Xml::Node vc_node = node.child("visual");
        settings.visual = Xml::Configs::get_VisualConfig_from_node(vc_node);        
    }
    
    // Random
    Random::Mt19937 prng(settings.pseudo_random_number_seed);

    // load scenery
    Frame world;
    world.set_name_pos_rot("root", Vec3::null, Rot3::null);
    Xml::SceneryFactory fab(scenery_path.path);
    fab.add_scenery_to_frame(&world);
    world.init_tree_based_on_mother_child_relations();
    // init Telescope Array Control
    TelescopeArrayControl* array_ctrl = fab.telescopes;

    // load photons
    EventIo::Run corsika_run(photon_path.path);

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
            
            vector<Photon> photons;
            EventIo::PhotonFactory cpf(corsika_photon, id++, &prng);

            if(cpf.passed_atmosphere()) {
                photons.push_back(cpf.get_photon());
            
                // propagate the cherenkov photons in the world
                Photons::propagate_photons_in_scenery_with_settings(
                    &photons, &world, &settings, &prng
                );

                for(const Photon &ph: photons) {
                    TrajectoryFactory traj(&ph);
                    traj.append_trajectory_to(&world);
                    world.init_tree_based_on_mother_child_relations();
                    free_orb.continue_with_new_scenery_and_settings(&world, &settings);
                    traj.erase_trajectory_from(&world);
                }
            }
        }
    }

    }catch(std::exception &error) {
        std::cerr << error.what(); 
    }
    return 0;
}
