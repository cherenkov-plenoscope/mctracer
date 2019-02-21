#include "DocOpt/docopt.h"
#include "Tools/PathTools.h"
#include "Scenery/StereoLitography/StereoLitography.h"
using std::string;
using std::cout;
using std::vector;
using namespace relleums;

static const char USAGE[] =
R"(Scale up or down the geometry of a STL CAD file.

    Usage:
      mctScaleStl --input=INPUT_STL_PATH --output=OUTPUT_STL_PATH --scale=SCALING_FACTOR
      mctScaleStl (-h | --help)
      mctScaleStl --version

    Options:
      -i --input=INPUT_STL_PATH     Original STL CAD file path.
      -o --output=OUTPUT_STL_PATH   Scaled output STL CAD file path.
      -s --scale=SCALING_FACTOR     Scaling factor to shrink or enlarge the geometry.
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

    const PathTools::Path input_path = PathTools::Path(
        args.find("--input")->second.asString());
    const PathTools::Path output_path = PathTools::Path(
        args.find("--output")->second.asString());

    const double scale = StringTools::to_double(
        args.find("--scale")->second.asString());

    StereoLitography::BinaryReader input = StereoLitography::BinaryReader(input_path.path);
    vector<StereoLitography::Facet> facets = input.get_facets();

    for(unsigned int i=0; i<facets.size(); i++) {
        facets.at(i).a = facets.at(i).a*scale;
        facets.at(i).b = facets.at(i).b*scale;
        facets.at(i).c = facets.at(i).c*scale;
    }

    StereoLitography::BinaryWriter output;
    output.add_facets(facets);
    output.write_to_file(output_path.path);

    }catch(std::exception &error) {
        std::cerr << error.what(); 
    }
    return 0;
}