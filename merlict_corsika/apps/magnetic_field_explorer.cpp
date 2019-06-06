// Copyright 2015 Sebastian A. Mueller
#include <stdint.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "merlict_corsika/eventio.h"
#include "merlict_corsika/corsika.h"
#include "merlict/numeric.h"

namespace ml = merlict;

std::vector<double> slice_photons(
    const std::vector<std::array<float, 8>> &photons,
    const uint64_t column_idx) {
    std::vector<double> column;
    for (uint64_t i = 0; i < photons.size(); i++) {
        column.push_back(photons.at(i).at(column_idx));
    }
    return column;
}


struct EventSummary {
    float run_num;
    float event_num;
    float particle_id;
    float particle_energy;

    float particle_momentum_px;
    float particle_momentum_py;
    float particle_momentum_pz;

    float particle_zenith_theta;
    float particle_azimuth_phi;

    float particle_core_x;
    float particle_core_y;

    float num_photons;
    float height_observation_level;

    float xs_median;
    float xs_mean;
    float xs_std;

    float ys_median;
    float ys_mean;
    float ys_std;

    float cxs_median;
    float cxs_mean;
    float cxs_std;

    float cys_median;
    float cys_mean;
    float cys_std;
};


int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::stringstream info;
            info << "Expect 2 arguments. 1st in_path, 2nd out_path.\n";
            info << "But actually argc is '" << argc << "'\n";
            throw std::invalid_argument(info.str());}

        const std::string in_path = argv[1];
        const std::string out_path = argv[2];

        eventio::Run corsika_run(in_path);

        std::ofstream out_file;
        out_file.open(out_path, std::ios::out | std::ios::binary);
        if (!out_file.is_open()) {
            std::stringstream info;
            info << "Can not open out_file: '" << out_path << "'.\n";
            throw std::runtime_error(info.str());}


        while (corsika_run.has_still_events_left()) {
            eventio::Event event = corsika_run.next_event();
            EventSummary s;
            s.run_num = event.header.raw.at(44 - 1);;
            s.event_num = event.header.raw.at(2 - 1);;

            s.particle_id = event.header.raw.at(3 - 1);
            s.particle_energy = event.header.raw.at(4 - 1);

            s.particle_momentum_px = event.header.raw.at(8 - 1);
            s.particle_momentum_py = event.header.raw.at(9 - 1);
            s.particle_momentum_pz = event.header.raw.at(10 - 1);

            s.particle_zenith_theta = event.header.raw.at(11 - 1);
            s.particle_azimuth_phi = event.header.raw.at(12 - 1);

            float num_reuses = event.header.raw.at(98 - 1);
            if (num_reuses != 1.) {
                std::stringstream info;
                info << "Expect number of reuses to be 1.\n";
                info << "But actually num_reuse (field 98 in event-header) is ";
                info << num_reuses << ".\n";
                throw std::invalid_argument(info.str());}

            s.particle_core_x = event.header.raw.at(98 - 1 + 1);
            s.particle_core_y = event.header.raw.at(118 - 1 + 1);

            if (fabs(s.particle_core_x) > 1. || fabs(s.particle_core_y) > 1.) {
                std::stringstream info;
                info << "Expect core-position to be [0, 0].\n";
                info << "But actually it is ";
                info << "[" << s.particle_core_x << ", ";
                info << s.particle_core_y << "].\n";
                throw std::invalid_argument(info.str());}

            s.height_observation_level = event.header.raw.at(47 + 1 - 1);

            const uint64_t num_cherenkov_photons = event.photons.size();
            s.num_photons = static_cast<float>(num_cherenkov_photons);
            std::vector<double> xs = slice_photons(event.photons, 0u);
            s.xs_median = ml::numeric::median(xs);
            s.xs_mean = ml::numeric::mean(xs);
            s.xs_std = ml::numeric::stddev(xs);
            std::vector<double> ys = slice_photons(event.photons, 1u);
            s.ys_median = ml::numeric::median(ys);
            s.ys_mean = ml::numeric::mean(ys);
            s.ys_std = ml::numeric::stddev(ys);
            std::vector<double> cxs = slice_photons(event.photons, 2u);
            s.cxs_median = ml::numeric::median(cxs);
            s.cxs_mean = ml::numeric::mean(cxs);
            s.cxs_std = ml::numeric::stddev(cxs);
            std::vector<double> cys = slice_photons(event.photons, 3u);
            s.cys_median = ml::numeric::median(cys);
            s.cys_mean = ml::numeric::mean(cys);
            s.cys_std = ml::numeric::stddev(cys);

            out_file.write((char*)&s, 1u*sizeof(EventSummary));
        }
        out_file.close();
    }catch(std::exception &error) {
        std::cerr << error.what();
    }
    return 0;
}
