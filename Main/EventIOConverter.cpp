#include "DocOpt/docopt.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"

#include "Corsika/EventIo/EventIo.h"
#include "Corsika/EventIo/Export.h"
#include "Corsika/Tools.h"

#include "Tools/AsciiIo.h"
#include "Tools/FileTools.h"
#include "Tools/Tools.h"

#include "Tools/PathTools.h"
#include "Tools/HeaderBlock.h"

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

static const char USAGE[] =
R"(EventIO converter

    Usage:
      mctEventIOConverter -i=CORSIKA_PATH -o=OUTPUT_PATH
      mctEventIOConverter (-h | --help)
      mctEventIOConverter --version

    Options:
      -i --input=CORSIKA_PATH   EventIO CORSIKA run path.
      -o --output=OUTPUT_PATH   Output path.
      -h --help                 Show this screen.
      --version                 Show version.
      
)";

int main(int argc, char* argv[]) {
    try {

    std::map<std::string, docopt::value> args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true,        // show help if requested
        "mct 0.0"
    );  

    PathTools::Path input_path(args.find("--input")->second.asString());
    PathTools::Path out_path(args.find("--output")->second.asString());
    
    // 1) create output directory
    fs::create_directory(out_path.path);

    EventIo::Run corsika_run(input_path.path);

    HeaderBlock::write(
        corsika_run.header.raw, 
        PathTools::join(out_path.path, "corsika_run_header.bin"));

    uint event_counter = 1;
    while(corsika_run.has_still_events_left()) {

        EventIo::Event event = corsika_run.next_event();

        PathTools::Path event_path(
            PathTools::join(
                out_path.path, 
                std::to_string(event_counter)));

        fs::create_directory(event_path.path);

        HeaderBlock::write(
            event.header.raw, 
            PathTools::join(
                event_path.path, 
                "corsika_event_header.bin"));

        EventIo::write_raw_photons(
            event.photons,
            PathTools::join(event_path.path, "air_shower_photon_bunches.bin")
        );

        event_counter++;
    }

    }catch(std::exception &error) {

        std::cerr << error.what(); 
    }
    return 0;
}