// Copyright 2015 Sebastian A. Mueller
#include "SignalProcessing/Sampler.h"
#include <sstream>
#include "TimeLineAddition.h"
using std::vector;

namespace SignalProcessing {
namespace PhotoElectricConverter {

SamplerConfig::SamplerConfig() {
    exposure_time = 150e-9;
    sampling_frequency = 2e9;
    std_dev_noise = 0.0;
    pulse_template = &box_pulse_5ns;
}

Sampler::Sampler(const SamplerConfig* config) {
    this->config = config;
    if (config->std_dev_noise < 0.0) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "Expected config->std_dev_noise > 0.0, ";
        info << "but actual it is: " << config->std_dev_noise;
        info << "\n";
        throw std::invalid_argument(info.str());
    }
    if (config->sampling_frequency < 0.0) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "Expected config->sampling_frequency > 0.0, ";
        info << "but actual it is: " << config->sampling_frequency;
        info << "\n";
        throw std::invalid_argument(info.str());
    }
    if (config->exposure_time < 0.0) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "Expected config->exposure_time > 0.0, ";
        info << "but actual it is: " << config->exposure_time;
        info << "\n";
        throw std::invalid_argument(info.str());
    }
    number_of_slices = (unsigned int)(ceil(
        config->exposure_time*config->sampling_frequency));
}

vector<double> Sampler::time_line(
    const vector<double> *electric_pipeline,
    const DiscretSampling::LookUpTable* puls_look_up_table,
    Random::Generator* prng
) {
    vector<double> time_line;
    time_line.reserve(number_of_slices);
    // add noise
    for (unsigned int i = 0; i < number_of_slices; i++)
        time_line.push_back(prng->normal(0.0, config->std_dev_noise));
    // add pulses
    for (unsigned int p = 0; p < electric_pipeline->size(); p++) {
        const double start_pos_in_slices =
            electric_pipeline->at(p)*config->sampling_frequency;
        const unsigned int start_slice = floor(start_pos_in_slices);
        const double start_offset_in_slices = fmod(start_pos_in_slices, 1);
        const vector<double> *pulse =
            puls_look_up_table->at(start_offset_in_slices);

        add_second_to_first_at(&time_line, pulse, start_slice);
    }
    return time_line;
}

unsigned int Sampler::get_number_of_slices()const {
    return number_of_slices;
}

}  // namespace PhotoElectricConverter
}  // namespace SignalProcessing
