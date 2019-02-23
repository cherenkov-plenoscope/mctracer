#include "DocOpt/docopt.h"
#include "Core/Photons.h"
#include "PhotonSensor/PhotonSensor.h"
#include "eventio.h"
#include "Corsika/corsika.h"
#include "Corsika/PhotonFactory.h"
#include "Visual/FlyingCamera.h"
#include "Scenery/TrajectoryFactory.h"
#include "Scenery/Scenery.h"
#include "Tools/PathTools.h"
#include "json.h"
using std::string;
using std::cout;
using std::array;
using std::vector;
using namespace relleums;

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
      mctShow --scenery=SCENERY_PATH --input=PHOTON_PATH [--config=CONFIG_PATH] [-r=SEED]
      mctShow (-h | --help)
      mctShow --version

    Options:
      -s --scenery=SCENERY_PATH     Scenery file path.
      -i --input=INPUT_PATH         CORSIKA run path.
      -c --config=CONFIG_PATH       Visual config file path.
      -r --random_seed=SEED         Seed for pseudo random number generator.
      -h --help                     Show this screen.
      --version                     Show version.
)";

int main(int argc, char* argv[]) {
    try{

    std::map<std::string, docopt::value> args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true,        // show help if requested
        "mct 0.1");  // version string

    path::Path scenery_path = path::Path(
        args.find("--scenery")->second.asString());
    path::Path photon_path = path::Path(
        args.find("--input")->second.asString());

    visual::Config visual_config;
    if (args.find("--config")->second) {
      visual_config = json::load_visual_config(
        args.find("--config")->second.asString());}

    PropagationConfig settings;

    Random::Mt19937 prng;
    if(args.find("--random_seed")->second)
        prng.set_seed(args.find("--random_seed")->second.asLong());

    Scenery scenery;
    json::append_to_frame_in_scenery(
      &scenery.root, &scenery, scenery_path.path);

    scenery.root.init_tree_based_on_mother_child_relations();

    eventio::Run corsika_run(photon_path.path);

    visual::FlyingCamera free_orb(&scenery.root, &visual_config);

    unsigned int event_counter = 0;
    while(corsika_run.has_still_events_left()) {
        event_counter++;

        // read next event
        eventio::Event event = corsika_run.next_event();

        // point the telescope into shower direction
        // double az = Corsika::EventHeader::azimuth(event.header.raw);
        // double zd = Corsika::EventHeader::zenith(event.header.raw);

        unsigned int id = 0;
        for(array<float, 8> corsika_photon : event.photons) {
            vector<Photon> photons;
            EventIoPhotonFactory cpf(corsika_photon, id++, &prng);

            if(cpf.passed_atmosphere()) {
                photons.push_back(cpf.get_photon());

                // propagate the cherenkov photons in the scenery
                Photons::propagate_photons_in_scenery_with_settings(
                    &photons, &scenery.root, &settings, &prng);

                for(const Photon &ph: photons) {
                    TrajectoryFactory traj(&ph);
                    traj.set_trajectory_radius(
                        visual_config.photon_trajectories.radius);
                    traj.append_trajectory_to(&scenery.root);
                    scenery.root.init_tree_based_on_mother_child_relations();
                    free_orb.continue_with_new_scenery_and_visual_config(
                        &scenery.root, 
                        &visual_config);
                    traj.erase_trajectory_from(&scenery.root);
                }
            }
        }
    }

    }catch(std::exception &error) {
        std::cerr << error.what();
    }
    return 0;
}
