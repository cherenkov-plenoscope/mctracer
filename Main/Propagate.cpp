#include "DocOpt/docopt.h"
#include "Core/Photons.h"
#include "Tools/AsciiIo.h"
#include "Xml/Factory/SceneryFactory.h"
#include "PhotonSensor/PhotonSensor.h"
#include "Xml/Factory/PropagationConfigFab.h"
#include "PhotonsReader/PhotonsReader.h"
#include "Scenery/Scenery.h"
using std::string;
using std::cout;
using std::vector;

static const char USAGE[] =
R"(Photon propagation

    Usage:
      mctPropagate -s=SCENERY_PATH -c=CONFIG_PATH -i=INPUT_PATH -o=OUTPUT_PATH [-r=SEED] [-b]
      mctPropagate (-h | --help)
      mctPropagate --version

    Options:
      -s --scenery=SCENERY_PATH Scenery xml file path.
      -c --config=CONFIG_PATH   Config path to xml file steering the simulation.
      -i --input=INPUT_PATH     Photon file path (e.g. a CORSIKA run).
      -o --output=OUTPUT_PATH   Output path.
      -r --random_seed=SEED     Seed for pseudo random number generator.
      -b --binary               Exports a binary float32 table for each sensor.
      -h --help                 Show this screen.
      --version                 Show version.
)";

int main(int argc, char* argv[]) {
    try{

    std::map<std::string, docopt::value> args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true,        // show help if requested
        "mct 0.0"
    );  // version string

    PathTools::Path out_path = PathTools::Path(args.find("--output")->second.asString());
    PathTools::Path scenery_path = PathTools::Path(args.find("--scenery")->second.asString());
    PathTools::Path photon_path = PathTools::Path(args.find("--input")->second.asString());
    PathTools::Path config_path = PathTools::Path(args.find("--config")->second.asString());
    const bool export_binary = args.find("--binary")->second.asBool();
    std::cout << "exp bin: " << export_binary << "\n";
    Xml::Document doc(config_path.path);
    Xml::Node se = doc.node().child("settings");
   
    //--------------------------------------------------------------------------
    // BASIC SETTINGS
    PropagationConfig settings = Xml::Configs::get_PropagationConfig_from_node(se);
    
    //--------------------------------------------------------------------------
    // Random
    Random::Mt19937 prng;
    if(args.find("--random_seed")->second)
        prng.set_seed(args.find("--random_seed")->second.asLong());

    //--------------------------------------------------------------------------
    // scenery
    Xml::SceneryFactory fab(scenery_path.path);
    Scenery scenery;
    fab.append_to_frame_in_scenery(&scenery.root, &scenery);
    scenery.root.init_tree_based_on_mother_child_relations();
    //--------------------------------------------------------------------------
    // sensors in scenery
    PhotonSensor::Sensors sensors = fab.sensors();
    
    //--------------------------------------------------------------------------
    // photon source
    PhotonsReader photon_file(photon_path.path);

    unsigned int event_counter = 1;
    while(photon_file.has_still_photons_left()) {

        vector<Photon> photons;
        photons = photon_file.next(&prng);

        //----------------------------------------------------------------------
        // photon propagation
        Photons::propagate_photons_in_scenery_with_settings(
            &photons, &scenery.root, &settings, &prng
        );

        //----------------------------------------------------------------------
        // detect photons in sensors
        sensors.clear_history();
        sensors.assign_photons(&photons);
        //----------------------------------------------------------------------
        // write each sensors to file
        for(unsigned int i=0; i<sensors.size(); i++) {

            std::stringstream outname;
            outname << out_path.path << event_counter << "_" << i;

            if(export_binary) {

                std::ofstream out;
                out.open(outname.str(), std::ios::out | std::ios::binary);
                PhotonSensor::write_arrival_information_to_file(
                    &(sensors.at(i)->photon_arrival_history),
                    &out);
                out.close();
            }else{

                std::stringstream header;
                header << "scenery: " << scenery_path.path << "\n";
                header << "sensor:  " << sensors.at(i)->frame->get_path_in_tree_of_frames() << ", ID: " << i << "\n";
                header << "photons: " << photon_path.path << "\n";
                header << "-------------\n";
                header << PhotonSensor::arrival_table_header();

                AsciiIo::write_table_to_file_with_header(
                    PhotonSensor::history_to_table(
                        sensors.at(i)->photon_arrival_history),
                    outname.str(),
                    header.str()
                );      
            }
        }
        event_counter++;
    }
    //--------------------------------------------------------------------------
    }catch(std::exception &error) {
        std::cerr << error.what(); 
    }
    return 0;
}