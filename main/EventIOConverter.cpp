// Copyright 2015 Sebastian A. Mueller
#include <experimental/filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include "docopt/docopt.h"
#include "eventio.h"
#include "corsika/block.h"
#include "merlict/merlict.h"

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

    mct::ospath::Path input_path(args.find("--input")->second.asString());
    mct::ospath::Path out_path(args.find("--output")->second.asString());

    fs::create_directory(out_path.path);

    eventio::Run corsika_run(input_path.path);

    corsika::block::write(
      corsika_run.header.raw,
      mct::ospath::join(out_path.path, "corsika_run_header.bin"));

    unsigned int event_counter = 1;
    while (corsika_run.has_still_events_left()) {
      eventio::Event event = corsika_run.next_event();

      mct::ospath::Path event_path(
        mct::ospath::join(out_path.path, std::to_string(event_counter)));

      fs::create_directory(event_path.path);

      corsika::block::write(
        event.header.raw,
        mct::ospath::join(event_path.path, "corsika_event_header.bin"));

      eventio::write_photon_bunches_to_path(
        event.photons,
        mct::ospath::join(event_path.path, "air_shower_photon_bunches.bin"));

      event_counter++;
    }
  }catch(std::exception &error) {
    std::cerr << error.what();
  }
  return 0;
}
