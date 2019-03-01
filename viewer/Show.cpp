#include "docopt/docopt.h"
#include "merlict/merlict.h"
#include "json.h"
#include "FlyingCamera.h"
namespace mct = merlict;


static const char USAGE[] =
R"(Show a scenery

  Usage:
    mctShow --scenery=SCENERY_PATH [--config=CONFIG_PATH]
    mctShow (-h | --help)
    mctShow --version

  Options:
    -s --scenery=SCENERY_PATH     Scenery file path.
    -c --config=CONFIG_PATH       Visual config file path.
    -h --help                     Show this screen.
    --version                     Show version.
)";

int main(int argc, char* argv[]) {
  try {
    std::map<std::string, docopt::value> args = docopt::docopt(
      USAGE,
      {argv + 1, argv + argc},
      true,        // show help if requested
      "mct 0.1");  // version string

    mct::ospath::Path scenery_path = mct::ospath::Path(
      args.find("--scenery")->second.asString());

    mct::Scenery scenery;

    mct::json::append_to_frame_in_scenery(
      &scenery.root, &scenery, scenery_path.path);

    mct::visual::Config visual_config;
    if (args.find("--config")->second) {
      visual_config = mct::json::load_visual_config(
        args.find("--config")->second.asString());}

    scenery.root.init_tree_based_on_mother_child_relations();
    mct::visual::FlyingCamera free(&scenery.root, &visual_config);

  } catch (std::exception &error) {
    std::cerr << error.what();
  }
  return 0;
}