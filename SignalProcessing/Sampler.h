// Copyright 2015 Sebastian A. Mueller
#ifndef MCTRACER_SIGNALPROCESSING_SAMPLER_H_
#define MCTRACER_SIGNALPROCESSING_SAMPLER_H_

#include <vector>
#include "Core/Function/Function.h"
#include "Core/Random/Random.h"
#include "PipelinePhoton.h"
#include "DiscretSampling.h"

namespace SignalProcessing {
namespace PhotoElectricConverter {

struct SamplerConfig {
    double exposure_time;
    double sampling_frequency;
    double std_dev_noise;
    const Function::Func1D* pulse_template;
    SamplerConfig();
};

static const Function::Constant box_pulse_5ns(
    1.0,
    Function::Limits(0.0e-9, 5.0e-9));

class Sampler {
    unsigned int number_of_slices;
    const SamplerConfig* config;
 public:
    explicit Sampler(const SamplerConfig* config);
    std::vector<double> time_line(
        const std::vector<double> *electric_pipeline,
        const DiscretSampling::LookUpTable* puls_look_up_table,
        Random::Generator* prng);
    unsigned int get_number_of_slices()const;
};

}  // namespace PhotoElectricConverter
}  // namespace SignalProcessing

#endif  // MCTRACER_SIGNALPROCESSING_SAMPLER_H_
