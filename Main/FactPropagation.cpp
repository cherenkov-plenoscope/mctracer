// Copyright 2014 Sebastian A. Mueller
#include <experimental/filesystem>
#include <algorithm>
#include "DocOpt/docopt.h"
#include "Main/photon_stream.h"
#include "Core/mctracer.h"
#include "eventio.h"
#include "Corsika/corsika.h"
#include "Corsika/PhotonFactory.h"
#include "Tools/PathTools.h"
#include "Scenery/Scenery.h"
#include "sensor/PhotonSensor.h"
#include "Scenery/Primitive/Primitive.h"
#include "Scenery/SegmentedReflector/SegmentedReflector.h"
#include "signal_processing/signal_processing.h"
#include "Visual/FlyingCamera.h"
namespace fs = std::experimental::filesystem;
namespace ps = photon_stream;
using namespace relleums;
using std::string;
using std::vector;
using std::array;
using std::cout;

static const char USAGE[] =
R"(FACT IACT air showher propagation

    Usage:
      mctFactPropagation -i=CORSIKA_PATH -o=OUTPUT_PATH [-r=SEED] [--all_truth]
      mctFactPropagation (-h | --help)
      mctFactPropagation --version

    Options:
      -i --input=CORSIKA_PATH   CORSIKA run path.
      -o --output=OUTPUT_PATH   Output path.
      -r --random_seed=SEED     Seed for pseudo random number generator.
      -h --help                 Show this screen.
      --version                 Show version.
)";

double estimate_median_arrival_time(
    const vector<vector<signal_processing::ElectricPulse>>& electric_pipelines
) {
    unsigned int num_pulses = 0;
    for (unsigned int chid = 0; chid < electric_pipelines.size(); chid++) {
        num_pulses += electric_pipelines.at(chid).size();
    }

    if (num_pulses == 0)
        return std::nan("");

    vector<double> all_arrival_times;
    all_arrival_times.reserve(num_pulses);

    for (unsigned int chid = 0; chid < electric_pipelines.size(); chid++) {
        for (unsigned int p = 0; p < electric_pipelines.at(chid).size(); p ++) {
            all_arrival_times.push_back(
                electric_pipelines.at(chid).at(p).arrival_time);
        }
    }

    std::sort(all_arrival_times.begin(), all_arrival_times.end());
    unsigned int half_num_pulses = static_cast<unsigned int>(
        static_cast<double>(num_pulses)*0.5);

    return all_arrival_times.at(half_num_pulses);
}

vector<uint8_t> electric_pipelines_to_photon_stream(
    const vector<vector<signal_processing::ElectricPulse>>& electric_pipelines,
    const double trigger_time=25e-9
) {
    const double median_arrival_time = estimate_median_arrival_time(
            electric_pipelines);

    vector<uint8_t> raw_phs;
    for (unsigned int chid = 0; chid < electric_pipelines.size(); chid++) {
        for (
            unsigned int p = 0;
            p < electric_pipelines.at(chid).size();
            p ++
        ) {
            double arrival_time =
                electric_pipelines.at(chid).at(p).arrival_time -
                median_arrival_time +
                trigger_time;
            int slice = static_cast<int>(
                round(arrival_time/ps::TIME_SLICE_DURATION_S));
            if (
                slice >=
                ps::NUMBER_OF_TIME_SLICES_OFFSET_AFTER_BEGIN_OF_ROI
                &&
                slice < ps::NUMBER_OF_TIME_SLICES +
                ps::NUMBER_OF_TIME_SLICES_OFFSET_AFTER_BEGIN_OF_ROI
            )
                raw_phs.push_back(static_cast<uint8_t>(slice));
        }
        raw_phs.push_back(ps::NEXT_PIXEL_MARKER);
    }
    return raw_phs;
}

void append_header_block(const array<float, 273>& block, std::ofstream& fout) {
    fout.write(
        reinterpret_cast<const char*>(&block),
        sizeof(array<float, 273>));
}

std::vector<std::vector<double>> pde_hamamatsu_S10362_33_050C() {
    std::vector<std::vector<double>> pde = {
        {2.00e-07, 0.000e+00},  // manual
        {3.00e-07, 0.000e+00},
        {3.19e-07, 7.581e-02},
        {3.24e-07, 1.242e-01},
        {3.33e-07, 1.864e-01},
        {3.40e-07, 2.200e-01},
        {3.44e-07, 2.311e-01},
        {3.51e-07, 2.417e-01},
        {3.59e-07, 2.498e-01},
        {3.78e-07, 2.672e-01},
        {3.89e-07, 2.914e-01},
        {3.92e-07, 2.958e-01},
        {4.00e-07, 3.014e-01},
        {4.25e-07, 3.200e-01},
        {4.32e-07, 3.287e-01},
        {4.44e-07, 3.300e-01},
        {4.62e-07, 3.281e-01},
        {4.89e-07, 3.119e-01},
        {5.00e-07, 3.051e-01},
        {5.54e-07, 2.504e-01},
        {5.77e-07, 2.262e-01},
        {6.00e-07, 2.044e-01},
        {6.16e-07, 1.870e-01},
        {6.49e-07, 1.566e-01},
        {6.90e-07, 1.236e-01},
        {7.00e-07, 1.187e-01},
        {7.48e-07, 9.073e-02},
        {7.88e-07, 7.022e-02},
        {8.09e-07, 6.214e-02},
        {8.55e-07, 4.163e-02},
        {9.00e-07, 2.734e-02},
        {1.00e-06, 1.350e-02},  // manual
        {1.10e-06, 0.700e-02},  // manual
        {1.20e-06, 0.000e-02},  // manual
    };
    return pde;
}

std::vector<std::vector<double>> fact_mirror_reflectivity() {
    std::vector<std::vector<double>> refl = {
       {2.00e-07, 5.271e-01},
       {2.05e-07, 5.988e-01},
       {2.10e-07, 6.569e-01},
       {2.15e-07, 6.987e-01},
       {2.20e-07, 7.260e-01},
       {2.25e-07, 7.423e-01},
       {2.30e-07, 7.509e-01},
       {2.39e-07, 7.517e-01},
       {2.44e-07, 7.458e-01},
       {2.49e-07, 7.365e-01},
       {2.54e-07, 7.250e-01},
       {2.59e-07, 7.134e-01},
       {2.64e-07, 7.039e-01},
       {2.73e-07, 6.961e-01},
       {2.78e-07, 6.991e-01},
       {2.83e-07, 7.075e-01},
       {2.88e-07, 7.202e-01},
       {2.93e-07, 7.361e-01},
       {2.98e-07, 7.534e-01},
       {3.07e-07, 7.846e-01},
       {3.12e-07, 8.007e-01},
       {3.17e-07, 8.152e-01},
       {3.22e-07, 8.255e-01},
       {3.27e-07, 8.368e-01},
       {3.32e-07, 8.466e-01},
       {3.41e-07, 8.610e-01},
       {3.46e-07, 8.674e-01},
       {3.51e-07, 8.726e-01},
       {3.56e-07, 8.772e-01},
       {3.61e-07, 8.810e-01},
       {3.66e-07, 8.842e-01},
       {3.75e-07, 8.886e-01},
       {3.80e-07, 8.912e-01},
       {3.85e-07, 8.930e-01},
       {3.90e-07, 8.938e-01},
       {3.95e-07, 8.949e-01},
       {4.00e-07, 8.958e-01},
       {4.09e-07, 8.965e-01},
       {4.14e-07, 8.966e-01},
       {4.19e-07, 8.967e-01},
       {4.24e-07, 8.966e-01},
       {4.29e-07, 8.962e-01},
       {4.34e-07, 8.957e-01},
       {4.43e-07, 8.941e-01},
       {4.48e-07, 8.938e-01},
       {4.53e-07, 8.940e-01},
       {4.58e-07, 8.942e-01},
       {4.63e-07, 8.940e-01},
       {4.68e-07, 8.927e-01},
       {4.77e-07, 8.890e-01},
       {4.82e-07, 8.863e-01},
       {4.87e-07, 8.840e-01},
       {4.92e-07, 8.834e-01},
       {4.97e-07, 8.831e-01},
       {5.02e-07, 8.826e-01},
       {5.11e-07, 8.793e-01},
       {5.16e-07, 8.771e-01},
       {5.21e-07, 8.745e-01},
       {5.26e-07, 8.723e-01},
       {5.31e-07, 8.696e-01},
       {5.36e-07, 8.674e-01},
       {5.45e-07, 8.627e-01},
       {5.50e-07, 8.598e-01},
       {5.55e-07, 8.574e-01},
       {5.60e-07, 8.545e-01},
       {5.65e-07, 8.518e-01},
       {5.70e-07, 8.490e-01},
       {5.79e-07, 8.441e-01},
       {5.84e-07, 8.410e-01},
       {5.89e-07, 8.386e-01},
       {5.94e-07, 8.354e-01},
       {5.99e-07, 8.325e-01},
       {6.04e-07, 8.298e-01},
       {6.13e-07, 8.241e-01},
       {6.18e-07, 8.216e-01},
       {6.23e-07, 8.179e-01},
       {6.28e-07, 8.151e-01},
       {6.33e-07, 8.125e-01},
       {6.38e-07, 8.089e-01},
       {6.47e-07, 8.037e-01},
       {6.52e-07, 8.005e-01},
       {6.57e-07, 7.972e-01},
       {6.62e-07, 7.944e-01},
       {6.67e-07, 7.913e-01},
       {6.72e-07, 7.883e-01},
       {6.81e-07, 7.827e-01},
       {6.86e-07, 7.798e-01},
       {6.91e-07, 7.771e-01},
       {6.96e-07, 7.740e-01},
       {7.01e-07, 7.708e-01},
       {7.06e-07, 7.682e-01},
       {7.15e-07, 7.629e-01},
       {7.20e-07, 7.599e-01},
       {7.25e-07, 7.578e-01},
       {7.30e-07, 7.552e-01},
       {7.35e-07, 7.518e-01},
       {7.40e-07, 7.492e-01},
       {7.49e-07, 7.457e-01},
       {7.54e-07, 7.428e-01},
       {7.59e-07, 7.399e-01},
       {7.64e-07, 7.382e-01},
       {7.69e-07, 7.368e-01},
       {7.74e-07, 7.351e-01},
       {7.83e-07, 7.312e-01},
       {7.88e-07, 7.300e-01},
       {7.93e-07, 7.292e-01},
       {7.98e-07, 7.286e-01},
       {8.00e-07, 7.282e-01},
       {9.00e-07, 6.850e-01},
       {9.50e-07, 6.634e-01}
    };
    return refl;
}


int main(int argc, char* argv[]) {
    try {
    std::map<std::string, docopt::value> args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true,        // show help if requested
        "mct 0.0");  // version string

    path::Path out_path = path::Path(args.find("--output")->second.asString());
    path::Path input_path = path::Path(args.find("--input")->second.asString());

    //--------------------------------------------------------------------------
    //  111
    // 1111      set up FACT
    //   11
    //   11
    //   11
    //   11
    // 111111 11
    //--------------------------------------------------------------------------
    Scenery scenery;
    scenery.root.set_name_pos_rot("root", VEC3_ORIGIN, ROT3_UNITY);
    scenery.functions.add(
      "mirror_reflectivity",
      function::Func1(fact_mirror_reflectivity()));
    scenery.colors.add("mirror_color", Color(128, 128, 128));
    scenery.colors.add("inner_mirror_color", Color(255, 255, 255));
    scenery.colors.add("pixel_red", Color(255, 0, 0));

    SegmentedReflector::Config r_cfg;
    r_cfg.focal_length = 4.889;
    r_cfg.DaviesCotton_over_parabolic_mixing_factor = 0.5;
    r_cfg.max_outer_aperture_radius = 2.1;
    r_cfg.min_inner_aperture_radius = 0.25;
    r_cfg.facet_inner_hex_radius = 0.3;
    r_cfg.gap_between_facets = 0.01;
    r_cfg.mirror_color = scenery.colors.get("mirror_color");
    r_cfg.inner_mirror_color = scenery.colors.get("inner_mirror_color");
    r_cfg.reflectivity = scenery.functions.get("mirror_reflectivity");

    SegmentedReflector::Factory r_factory = SegmentedReflector::Factory(r_cfg);
    r_factory.add_reflector_mirror_facets_to_frame(
        &scenery.root);

    Frame* camera = scenery.root.append<Frame>();
    camera->set_name_pos_rot("camera", Vec3(0, 0, 4.889), ROT3_UNITY);
    Disc* shield = camera->append<Disc>();
    shield->set_name_pos_rot("shield", Vec3(0, 0, 0.001), ROT3_UNITY);
    shield->set_radius(0.33);

    const unsigned int NUM_PIXEL = 1440;
    array<float, NUM_PIXEL> pixel_x = {28.5,33.25,33.25,38.0,42.75,42.75,47.5,52.25,52.25,-0.0,4.75,4.75,9.5,14.25,14.25,19.0,23.75,23.75,71.25,66.5,66.5,61.75,57.0,57.0,52.25,47.5,47.5,42.75,38.0,38.0,33.25,28.5,28.5,23.75,19.0,19.0,114.0,109.25,109.25,104.5,99.75,99.75,95.0,90.25,90.25,133.0,128.25,123.5,118.75,123.5,118.75,114.0,109.25,104.5,104.5,99.75,95.0,90.25,95.0,90.25,85.5,80.75,76.0,85.5,80.75,76.0,76.0,71.25,66.5,61.75,61.75,57.0,128.25,123.5,123.5,118.75,114.0,114.0,109.25,104.5,104.5,152.0,147.25,142.5,137.75,147.25,142.5,137.75,133.0,128.25,99.75,95.0,95.0,90.25,85.5,85.5,80.75,76.0,76.0,71.25,66.5,66.5,61.75,57.0,57.0,52.25,47.5,47.5,85.5,80.75,80.75,76.0,71.25,71.25,66.5,61.75,61.75,57.0,52.25,52.25,47.5,42.75,42.75,38.0,33.25,33.25,28.5,23.75,23.75,19.0,14.25,14.25,9.5,4.75,4.75,42.75,38.0,38.0,33.25,28.5,28.5,23.75,19.0,19.0,175.75,175.75,171.0,166.25,161.5,161.5,156.75,152.0,147.25,171.0,166.25,161.5,156.75,156.75,152.0,147.25,142.5,133.0,142.5,137.75,137.75,133.0,128.25,128.25,123.5,118.75,118.75,114.0,109.25,109.25,104.5,99.75,99.75,95.0,90.25,90.25,85.5,80.75,80.75,76.0,71.25,71.25,66.5,61.75,61.75,57.0,52.25,52.25,47.5,42.75,42.75,38.0,33.25,33.25,28.5,23.75,23.75,19.0,14.25,14.25,9.5,4.75,4.75,14.25,9.5,9.5,4.75,-0.0,-0.0,-4.75,-9.5,-9.5,185.25,185.25,180.5,175.75,180.5,175.75,171.0,166.25,161.5,156.75,152.0,152.0,147.25,142.5,142.5,137.75,133.0,133.0,128.25,123.5,123.5,118.75,114.0,114.0,109.25,104.5,104.5,99.75,95.0,95.0,90.25,85.5,85.5,80.75,76.0,76.0,42.75,38.0,38.0,33.25,28.5,28.5,23.75,19.0,19.0,71.25,66.5,66.5,61.75,57.0,57.0,52.25,47.5,47.5,57.0,52.25,52.25,47.5,42.75,42.75,38.0,33.25,33.25,28.5,23.75,23.75,19.0,14.25,14.25,9.5,4.75,4.75,171.0,166.25,166.25,161.5,156.75,156.75,152.0,147.25,147.25,142.5,137.75,137.75,133.0,128.25,128.25,123.5,118.75,118.75,114.0,109.25,109.25,104.5,99.75,99.75,95.0,90.25,90.25,85.5,80.75,80.75,76.0,71.25,71.25,66.5,61.75,61.75,156.75,152.0,152.0,147.25,142.5,142.5,137.75,133.0,133.0,185.25,180.5,180.5,175.75,171.0,171.0,166.25,161.5,161.5,128.25,123.5,123.5,118.75,114.0,114.0,109.25,104.5,104.5,99.75,95.0,95.0,90.25,85.5,85.5,80.75,76.0,76.0,71.25,66.5,66.5,61.75,57.0,57.0,52.25,47.5,47.5,42.75,38.0,38.0,33.25,28.5,28.5,23.75,19.0,19.0,28.5,23.75,23.75,19.0,14.25,14.25,9.5,4.75,4.75,14.25,9.5,9.5,4.75,-0.0,-0.0,-4.75,-9.5,-9.5,85.5,80.75,80.75,76.0,71.25,71.25,66.5,61.75,61.75,57.0,52.25,52.25,47.5,42.75,42.75,38.0,33.25,33.25,42.75,38.0,38.0,33.25,28.5,28.5,23.75,19.0,19.0,14.25,9.5,9.5,4.75,-0.0,-0.0,-4.75,-9.5,-9.5,185.25,185.25,180.5,175.75,180.5,175.75,171.0,166.25,161.5,171.0,166.25,166.25,161.5,156.75,156.75,152.0,147.25,147.25,142.5,137.75,137.75,133.0,128.25,128.25,123.5,118.75,118.75,114.0,109.25,109.25,104.5,99.75,99.75,95.0,90.25,90.25,175.75,175.75,171.0,166.25,161.5,161.5,156.75,152.0,147.25,156.75,152.0,152.0,147.25,142.5,142.5,137.75,133.0,133.0,128.25,123.5,123.5,118.75,114.0,114.0,109.25,104.5,104.5,99.75,95.0,95.0,90.25,85.5,85.5,80.75,76.0,76.0,57.0,52.25,52.25,47.5,42.75,42.75,38.0,33.25,33.25,71.25,66.5,66.5,61.75,57.0,57.0,52.25,47.5,47.5,42.75,38.0,38.0,33.25,28.5,28.5,23.75,19.0,19.0,28.5,23.75,23.75,19.0,14.25,14.25,9.5,4.75,4.75,171.0,166.25,161.5,156.75,156.75,152.0,147.25,142.5,133.0,142.5,137.75,137.75,133.0,128.25,128.25,123.5,118.75,118.75,114.0,109.25,109.25,104.5,99.75,99.75,95.0,90.25,90.25,85.5,80.75,80.75,76.0,71.25,71.25,66.5,61.75,61.75,152.0,147.25,142.5,137.75,147.25,142.5,137.75,133.0,128.25,128.25,123.5,123.5,118.75,114.0,114.0,109.25,104.5,104.5,99.75,95.0,95.0,90.25,85.5,85.5,80.75,76.0,76.0,71.25,66.5,66.5,61.75,57.0,57.0,52.25,47.5,47.5,133.0,128.25,123.5,118.75,123.5,118.75,114.0,109.25,104.5,114.0,109.25,109.25,104.5,99.75,99.75,95.0,90.25,90.25,104.5,99.75,95.0,90.25,95.0,90.25,85.5,80.75,76.0,85.5,80.75,80.75,76.0,71.25,71.25,66.5,61.75,61.75,57.0,52.25,52.25,47.5,42.75,42.75,38.0,33.25,33.25,28.5,23.75,23.75,19.0,14.25,14.25,9.5,4.75,4.75,42.75,38.0,38.0,33.25,28.5,28.5,23.75,19.0,19.0,14.25,9.5,9.5,4.75,-0.0,-0.0,-4.75,-9.5,-9.5,85.5,80.75,76.0,76.0,71.25,66.5,61.75,61.75,57.0,71.25,66.5,66.5,61.75,57.0,57.0,52.25,47.5,47.5,28.5,33.25,33.25,38.0,42.75,42.75,47.5,52.25,52.25,-0.0,4.75,4.75,9.5,14.25,14.25,19.0,23.75,23.75,-57.0,-52.25,-52.25,-47.5,-42.75,-42.75,-38.0,-33.25,-33.25,-28.5,-23.75,-23.75,-19.0,-14.25,-14.25,-9.5,-4.75,-4.75,-42.75,-47.5,-47.5,-52.25,-57.0,-57.0,-61.75,-66.5,-66.5,-14.25,-19.0,-19.0,-23.75,-28.5,-28.5,-33.25,-38.0,-38.0,-85.5,-90.25,-90.25,-95.0,-99.75,-99.75,-104.5,-109.25,-109.25,-104.5,-114.0,-118.75,-123.5,-114.0,-118.75,-123.5,-128.25,-128.25,-71.25,-76.0,-80.75,-85.5,-90.25,-95.0,-99.75,-104.5,-109.25,-61.75,-66.5,-71.25,-76.0,-76.0,-80.75,-85.5,-90.25,-95.0,-99.75,-104.5,-104.5,-109.25,-114.0,-114.0,-118.75,-123.5,-123.5,-128.25,-133.0,-133.0,-137.75,-133.0,-137.75,-142.5,-142.5,-147.25,-71.25,-76.0,-76.0,-80.75,-85.5,-85.5,-90.25,-95.0,-95.0,-42.75,-47.5,-47.5,-52.25,-57.0,-57.0,-61.75,-66.5,-66.5,-57.0,-61.75,-61.75,-66.5,-71.25,-71.25,-76.0,-80.75,-80.75,-28.5,-33.25,-33.25,-38.0,-42.75,-42.75,-47.5,-52.25,-52.25,-14.25,-19.0,-19.0,-23.75,-28.5,-28.5,-33.25,-38.0,-38.0,-0.0,-4.75,-4.75,-9.5,-14.25,-14.25,-19.0,-23.75,-23.75,-147.25,-152.0,-156.75,-161.5,-166.25,-166.25,-171.0,-171.0,-175.75,-142.5,-147.25,-142.5,-156.75,-152.0,-152.0,-156.75,-147.25,-161.5,-114.0,-118.75,-118.75,-123.5,-128.25,-128.25,-133.0,-137.75,-137.75,-85.5,-90.25,-90.25,-95.0,-99.75,-99.75,-104.5,-109.25,-109.25,-57.0,-61.75,-61.75,-66.5,-71.25,-71.25,-76.0,-80.75,-80.75,-28.5,-33.25,-33.25,-38.0,-42.75,-42.75,-47.5,-52.25,-52.25,-0.0,-4.75,-4.75,-9.5,-14.25,-14.25,-19.0,-23.75,-23.75,14.25,9.5,9.5,4.75,-0.0,-0.0,-4.75,-9.5,-9.5,-156.75,-161.5,-161.5,-166.25,-171.0,-175.75,-175.75,-180.5,-185.25,-128.25,-133.0,-133.0,-137.75,-142.5,-142.5,-147.25,-152.0,-152.0,-99.75,-104.5,-104.5,-109.25,-114.0,-114.0,-118.75,-123.5,-123.5,-71.25,-76.0,-76.0,-80.75,-85.5,-85.5,-90.25,-95.0,-95.0,-42.75,-47.5,-47.5,-52.25,-57.0,-57.0,-61.75,-66.5,-66.5,-14.25,-19.0,-19.0,-23.75,-28.5,-28.5,-33.25,-38.0,-38.0,-28.5,-33.25,-33.25,-38.0,-42.75,-42.75,-47.5,-52.25,-52.25,-0.0,-4.75,-4.75,-9.5,-14.25,-14.25,-19.0,-23.75,-23.75,-142.5,-147.25,-147.25,-152.0,-156.75,-156.75,-161.5,-166.25,-166.25,-114.0,-118.75,-118.75,-123.5,-128.25,-128.25,-133.0,-137.75,-137.75,-85.5,-90.25,-90.25,-95.0,-99.75,-99.75,-104.5,-109.25,-109.25,-57.0,-61.75,-61.75,-66.5,-71.25,-71.25,-76.0,-80.75,-80.75,-156.75,-161.5,-161.5,-166.25,-171.0,-171.0,-175.75,-180.5,-185.25,-171.0,-171.0,-175.75,-180.5,-175.75,-180.5,-180.5,-185.25,-185.25,-128.25,-133.0,-133.0,-137.75,-142.5,-142.5,-147.25,-152.0,-152.0,-99.75,-104.5,-104.5,-109.25,-114.0,-114.0,-118.75,-123.5,-123.5,-71.25,-76.0,-76.0,-80.75,-85.5,-85.5,-90.25,-95.0,-95.0,-42.75,-47.5,-47.5,-52.25,-57.0,-57.0,-61.75,-66.5,-66.5,-14.25,-19.0,-19.0,-23.75,-28.5,-28.5,-33.25,-38.0,-38.0,-0.0,-4.75,-4.75,-9.5,-14.25,-14.25,-19.0,-23.75,-23.75,-57.0,-61.75,-61.75,-66.5,-71.25,-71.25,-76.0,-80.75,-80.75,-28.5,-33.25,-33.25,-38.0,-42.75,-42.75,-47.5,-52.25,-52.25,-14.25,-19.0,-19.0,-23.75,-28.5,-28.5,-33.25,-38.0,-38.0,14.25,9.5,9.5,4.75,-0.0,-0.0,-4.75,-9.5,-9.5,-156.75,-161.5,-161.5,-166.25,-171.0,-175.75,-175.75,-180.5,-185.25,-142.5,-147.25,-147.25,-152.0,-156.75,-156.75,-161.5,-166.25,-166.25,-114.0,-118.75,-118.75,-123.5,-128.25,-128.25,-133.0,-137.75,-137.75,-85.5,-90.25,-90.25,-95.0,-99.75,-99.75,-104.5,-109.25,-109.25,-147.25,-152.0,-156.75,-161.5,-166.25,-166.25,-171.0,-171.0,-175.75,-128.25,-133.0,-133.0,-137.75,-142.5,-142.5,-147.25,-152.0,-152.0,-99.75,-104.5,-104.5,-109.25,-114.0,-114.0,-118.75,-123.5,-123.5,-71.25,-76.0,-76.0,-80.75,-85.5,-85.5,-90.25,-95.0,-95.0,-28.5,-33.25,-33.25,-38.0,-42.75,-42.75,-47.5,-52.25,-52.25,-42.75,-47.5,-47.5,-52.25,-57.0,-57.0,-61.75,-66.5,-66.5,-14.25,-19.0,-19.0,-23.75,-28.5,-28.5,-33.25,-38.0,-38.0,-0.0,-4.75,-4.75,-9.5,-14.25,-14.25,-19.0,-23.75,-23.75,-142.5,-142.5,-147.25,-156.75,-147.25,-152.0,-152.0,-156.75,-161.5,-114.0,-118.75,-118.75,-123.5,-128.25,-128.25,-133.0,-137.75,-137.75,-85.5,-90.25,-90.25,-95.0,-99.75,-99.75,-104.5,-109.25,-109.25,-57.0,-61.75,-61.75,-66.5,-71.25,-71.25,-76.0,-80.75,-80.75,-128.25,-133.0,-133.0,-137.75,-133.0,-137.75,-142.5,-142.5,-147.25,-99.75,-104.5,-104.5,-109.25,-114.0,-114.0,-118.75,-123.5,-123.5,-71.25,-76.0,-76.0,-80.75,-85.5,-85.5,-90.25,-95.0,-95.0,-42.75,-47.5,-47.5,-52.25,-57.0,-57.0,-61.75,-66.5,-66.5,-104.5,-114.0,-118.75,-123.5,-114.0,-118.75,-123.5,-128.25,-128.25,-85.5,-90.25,-90.25,-95.0,-99.75,-99.75,-104.5,-109.25,-109.25,-71.25,-76.0,-80.75,-85.5,-90.25,-95.0,-99.75,-104.5,-109.25,-57.0,-61.75,-61.75,-66.5,-71.25,-71.25,-76.0,-80.75,-80.75,-28.5,-33.25,-33.25,-38.0,-42.75,-42.75,-47.5,-52.25,-52.25,-0.0,-4.75,-4.75,-9.5,-14.25,-14.25,-19.0,-23.75,-23.75,-14.25,-19.0,-19.0,-23.75,-28.5,-28.5,-33.25,-38.0,-38.0,14.25,9.5,9.5,4.75,-0.0,-0.0,-4.75,-9.5,-9.5,-61.75,-66.5,-71.25,-76.0,-76.0,-80.75,-85.5,-90.25,-95.0,-42.75,-47.5,-47.5,-52.25,-57.0,-57.0,-61.75,-66.5,-66.5,-57.0,-52.25,-52.25,-47.5,-42.75,-42.75,-38.0,-33.25,-33.25,-28.5,-23.75,-23.75,-19.0,-14.25,-14.25,-9.5,-4.75,-4.75};
    array<float, NUM_PIXEL> pixel_y = {172.805,180.975,164.54,172.805,180.975,164.54,172.805,180.975,164.54,172.805,180.975,164.54,172.805,180.975,164.54,172.805,180.975,164.54,148.105,139.84,156.275,148.105,139.84,156.275,148.105,139.84,156.275,148.105,139.84,156.275,148.105,139.84,156.275,148.105,139.84,156.275,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,131.67,123.805,115.14,139.84,131.67,139.84,148.105,139.84,156.275,148.105,156.275,164.54,139.84,148.105,139.84,148.105,139.84,156.275,164.54,156.275,172.805,164.54,172.805,164.54,180.975,172.805,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,106.97,98.705,106.97,98.705,115.14,123.805,115.14,106.97,115.14,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,49.4,65.835,57.57,65.835,57.57,74.005,65.835,74.005,65.835,74.005,82.27,90.535,82.27,98.705,90.535,82.27,90.535,90.535,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,16.435,32.87,24.7,16.435,41.135,32.87,41.135,49.4,41.135,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-32.87,-16.435,-24.7,-16.435,-41.135,-32.87,-41.135,-49.4,-41.135,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-65.835,-49.4,-57.57,-65.835,-74.005,-57.57,-65.835,-74.005,-65.835,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-90.535,-82.27,-98.705,-90.535,-82.27,-90.535,-90.535,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-106.97,-98.705,-106.97,-98.705,-115.14,-123.805,-115.14,-106.97,-115.14,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-123.805,-131.67,-123.805,-115.14,-139.84,-131.67,-139.84,-148.105,-139.84,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-156.275,-148.105,-156.275,-164.54,-139.84,-148.105,-139.84,-148.105,-139.84,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-156.275,-164.54,-172.805,-156.275,-164.54,-172.805,-180.975,-164.54,-172.805,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-172.805,-164.54,-180.975,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-148.105,-156.275,-139.84,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-139.84,-139.84,-131.67,-139.84,-123.805,-115.14,-123.805,-131.67,-115.14,-148.105,-139.84,-148.105,-139.84,-148.105,-139.84,-148.105,-156.275,-148.105,-164.54,-172.805,-164.54,-172.805,-156.275,-164.54,-156.275,-164.54,-156.275,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-123.805,-115.14,-123.805,-106.97,-115.14,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-123.805,-131.67,-115.14,-65.835,-74.005,-65.835,-74.005,-82.27,-65.835,-74.005,-57.57,-65.835,-90.535,-82.27,-74.005,-82.27,-90.535,-106.97,-98.705,-98.705,-90.535,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-74.005,-82.27,-65.835,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-98.705,-106.97,-90.535,-49.4,-57.57,-41.135,-49.4,-41.135,-49.4,-32.87,-41.135,-32.87,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-49.4,-57.57,-41.135,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,-24.7,-32.87,-16.435,0.0,-8.265,8.265,0.0,8.265,24.7,16.435,24.7,16.435,-24.7,-8.265,-16.435,-24.7,0.0,-8.265,8.265,-16.435,0.0,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,0.0,-8.265,8.265,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,32.87,49.4,41.135,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,24.7,16.435,32.87,65.835,74.005,65.835,74.005,65.835,82.27,57.57,74.005,65.835,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,49.4,41.135,57.57,49.4,41.135,57.57,49.4,41.135,57.57,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,90.535,82.27,82.27,98.705,90.535,106.97,98.705,90.535,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,74.005,65.835,82.27,98.705,90.535,106.97,98.705,123.805,115.14,106.97,123.805,115.14,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,98.705,90.535,106.97,139.84,139.84,131.67,139.84,123.805,115.14,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,148.105,139.84,148.105,139.84,148.105,139.84,148.105,156.275,148.105,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,123.805,115.14,131.67,148.105,139.84,156.275,148.105,139.84,156.275,148.105,139.84,156.275,148.105,139.84,156.275,148.105,139.84,156.275,148.105,139.84,156.275,164.54,172.805,164.54,172.805,156.275,164.54,156.275,164.54,156.275,148.105,139.84,156.275,148.105,139.84,156.275,148.105,139.84,156.275,172.805,180.975,164.54,172.805,180.975,164.54,172.805,180.975,164.54,172.805,180.975,164.54,172.805,180.975,164.54,172.805,180.975,164.54};

    vector<sensor::Sensor*> channels;

    for (
        unsigned int chid = 0;
        chid < NUM_PIXEL;
        chid++
    ) {
        double x = pixel_x[chid]/1e3;
        double y = pixel_y[chid]/1e3;

        HexPlane* pixel_aperture = camera->append<HexPlane>();
        pixel_aperture->set_name_pos_rot(
            "pixel_"+std::to_string(chid),
            Vec3(x, y, 0),
            Rot3(0, 0, deg2rad(30)));
        pixel_aperture->set_outer_hex_radius(0.00495);
        pixel_aperture->set_inner_color(scenery.colors.get("pixel_red"));
        pixel_aperture->set_outer_color(scenery.colors.get("pixel_red"));

        sensor::Sensor* pixel_sensor = new sensor::Sensor(
            chid,
            pixel_aperture);
        channels.push_back(pixel_sensor);
    }

    sensor::Sensors pixels = sensor::Sensors(channels);

    scenery.root.init_tree_based_on_mother_child_relations();
    // Visual::Config visual_config;
    // Visual::FlyingCamera free(&scenery.root, &visual_config);

    // -------------------------------------------------------------------------
    PropagationConfig settings;
    random::Mt19937 prng;
    if (args.find("--random_seed")->second)
        prng.set_seed(args.find("--random_seed")->second.asLong());
    function::Func1 pde_vs_wavelength = function::Func1(
        pde_hamamatsu_S10362_33_050C());

    signal_processing::PhotoElectricConverter::Config converter_config;
    converter_config.dark_rate = 0.0;
    converter_config.probability_for_second_puls = 0.0;
    converter_config.quantum_efficiency_vs_wavelength = &pde_vs_wavelength;
    signal_processing::PhotoElectricConverter::Converter sipm_converter(
        &converter_config);

    const double nsb_exposure_time = 1e-9;
    // -------------------------------------------------------------------------
    //  2222
    // 22  22      run the simulation
    //    22
    //   22
    //  22
    // 22
    // 222222 22
    // -------------------------------------------------------------------------
    // open cherenkov photon file
    eventio::Run corsika_run(input_path.path);

    std::ofstream fout(out_path.path.c_str(), std::ios::binary);
    std::ofstream fch((out_path.path+".ch").c_str(), std::ios::binary);
    append_header_block(corsika_run.header.raw, fch);

    // -------------------------------------------------------------------------
    unsigned int event_counter = 1;
    while (corsika_run.has_still_events_left()) {
        eventio::Event event = corsika_run.next_event();

        vector<Photon> photons;
        unsigned int photon_id = 0;

        for (const array<float, 8> &corsika_photon : event.photons) {
            EventIoPhotonFactory cpf(corsika_photon, photon_id++, &prng);
            if (cpf.passed_atmosphere())
                photons.push_back(cpf.get_photon());
        }

        Photons::propagate_photons_in_scenery_with_settings(
            &photons, &scenery.root, &settings, &prng);

        pixels.clear_history();
        pixels.assign_photons(&photons);

        vector<vector<signal_processing::PipelinePhoton>> photon_pipelines =
            signal_processing::get_photon_pipelines(&pixels);

        vector<vector<signal_processing::ElectricPulse>> electric_pipelines;
        electric_pipelines.reserve(photon_pipelines.size());
        for (vector<signal_processing::PipelinePhoton> ph_pipe : photon_pipelines
        ) {
            electric_pipelines.push_back(
                sipm_converter.get_pulse_pipeline_for_photon_pipeline(
                    ph_pipe,
                    nsb_exposure_time,
                    &prng));
        }

        ps::SimulationEvent phs_event;
        phs_event.descriptor.magic_1 = ps::MAGIC_DESCRIPTOR_1;
        phs_event.descriptor.magic_2 = ps::MAGIC_DESCRIPTOR_2;
        phs_event.descriptor.magic_3 = ps::MAGIC_DESCRIPTOR_3;
        phs_event.descriptor.pass_version = ps::PASS_VERSION;
        phs_event.descriptor.event_type = ps::SIMULATION_EVENT_TYPE_KEY;

        phs_event.id.run = Corsika::RunHeader::run_number(
            corsika_run.header.raw);
        phs_event.id.event = Corsika::EventHeader::event_number(
            event.header.raw);
        phs_event.id.reuse = 0u;

        phs_event.pointing.zd = 0.0;
        phs_event.pointing.az = 0.0;

        phs_event.photon_stream.raw = electric_pipelines_to_photon_stream(
            electric_pipelines);

        ps::append_SimulationEvent_to_file(phs_event, fout);
        append_header_block(event.header.raw, fch);

        cout << "event " << event_counter << ", ";
        cout << "PRMPAR ";
        cout << Corsika::EventHeader::particle_id(event.header.raw) << ", ";
        cout << "E ";
        cout << Corsika::EventHeader::total_energy_in_GeV(event.header.raw);
        cout << " GeV\n";
        event_counter++;
    }
    fout.close();

    array<float, 273> run_end;
    run_end[0] = Corsika::str2float("RUNE");
    run_end[1] = Corsika::RunHeader::run_number(corsika_run.header.raw);
    run_end[2] = static_cast<float>(event_counter);
    append_header_block(run_end, fch);

    fch.close();
    }catch(std::exception &error) {
        std::cerr << error.what();
    }
    return 0;
}
