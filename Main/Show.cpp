#include "DocOpt/docopt.h"
#include "Core/Photons.h"
#include "Scenery/StereoLitography/StereoLitography.h"
#include "Xml/Factory/VisualConfigFab.h"
#include "Xml/Factory/PropagationConfigFab.h"
#include "Xml/Factory/SceneryFactory.h"
#include "Cameras/FlyingCamera.h"
#include "Scenery/Scenery.h"
using std::string;
using std::cout;

static const char USAGE[] =
R"(Show a scenery

    Usage:
      mctShow --scenery=SCENERY_PATH [--config=CONFIG_PATH]
      mctShow (-h | --help)
      mctShow --version

    Options:
      -s --scenery=SCENERY_PATH     Scenery xml file path.
      -c --config=CONFIG_PATH       Visual config xml file path.
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

    Scenery scenery;
    if(
        StringTools::is_ending(scenery_path.path,".stl") || 
        StringTools::is_ending(scenery_path.path,".STL")
    ) {
        const double scale = 1.0;
        StereoLitography::add_stl_to_frame(scenery_path.path, &scenery.root, scale);
    }else if(   
        StringTools::is_ending(scenery_path.path,".xml") ||
        StringTools::is_ending(scenery_path.path,".XML")
    ) {
        Xml::SceneryFactory fab(scenery_path.path);
        fab.append_to_frame_in_scenery(&scenery.root, &scenery);
    }else{
        cout << "Can only read stl or xml files.\n";
        return 0;
    }
    
    VisualConfig visual_config;
    if(args.find("--config")->second) {
        Xml::Document doc(args.find("--config")->second.asString());
        Xml::Node node = doc.node();
        Xml::Node vc_node = node.child("visual");
        visual_config = Xml::Configs::get_VisualConfig_from_node(vc_node);        
    }

    scenery.root.init_tree_based_on_mother_child_relations();
    FlyingCamera free(&scenery.root, &visual_config);

    }catch(std::exception &error) {
        std::cerr << error.what(); 
    }
    return 0;
}