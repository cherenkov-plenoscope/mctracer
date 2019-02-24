#include <iostream>
#include "DocOpt/docopt.h"
#include "Tools/PathTools.h"
#include "scenery/stereo_litography.h"
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

    const path::Path input_path = path::Path(
        args.find("--input")->second.asString());
    const path::Path output_path = path::Path(
        args.find("--output")->second.asString());

    const double scale = txt::to_double(
        args.find("--scale")->second.asString());

    stereo_litography::BinaryReader input = stereo_litography::BinaryReader(input_path.path);
    vector<stereo_litography::Facet> facets = input.get_facets();

    for(unsigned int i=0; i<facets.size(); i++) {
        facets.at(i).a = facets.at(i).a*scale;
        facets.at(i).b = facets.at(i).b*scale;
        facets.at(i).c = facets.at(i).c*scale;
    }

    stereo_litography::BinaryWriter output;
    output.add_facets(facets);
    output.write_to_file(output_path.path);

    }catch(std::exception &error) {
        std::cerr << error.what(); 
    }
    return 0;
}