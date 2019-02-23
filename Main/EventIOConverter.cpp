// Copyright 2015 Sebastian A. Mueller
#include <experimental/filesystem>
#include <fstream>
#include <sstream>
#include "DocOpt/docopt.h"
#include "eventio.h"
#include "Corsika/EventIo/Export.h"
#include "Tools/PathTools.h"
#include "Tools/HeaderBlock.h"

namespace fs = std::experimental::filesystem;
namespace mct = relleums;

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
      "mct 0.0");

    mct::path::Path input_path(args.find("--input")->second.asString());
    mct::path::Path out_path(args.find("--output")->second.asString());

    fs::create_directory(out_path.path);

    eventio::Run corsika_run(input_path.path);

    mct::HeaderBlock::write(
      corsika_run.header.raw,
      mct::path::join(out_path.path, "corsika_run_header.bin"));

    unsigned int event_counter = 1;
    while (corsika_run.has_still_events_left()) {
      eventio::Event event = corsika_run.next_event();

      mct::path::Path event_path(
        mct::path::join(out_path.path, std::to_string(event_counter)));

      fs::create_directory(event_path.path);

      mct::HeaderBlock::write(
        event.header.raw,
        mct::path::join(event_path.path, "corsika_event_header.bin"));

      relleums::write_corsika_photons(
        event.photons,
        mct::path::join(event_path.path, "air_shower_photon_bunches.bin"));

      event_counter++;
    }
  }catch(std::exception &error) {
    std::cerr << error.what();
  }
  return 0;
}
