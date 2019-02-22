// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_SIGNALPROCESSING_PHOTOELECTRICCONVERTER_H_
#define MCTRACER_SIGNALPROCESSING_PHOTOELECTRICCONVERTER_H_

#include <vector>
#include "Core/Function/Function.h"
#include "Core/Random/Random.h"
#include "PipelinePhoton.h"
#include "ElectricPulse.h"


namespace SignalProcessing {
namespace PhotoElectricConverter {

struct Config {
    const relleums::Function::Func1* quantum_efficiency_vs_wavelength;
    double dark_rate;
    double probability_for_second_puls;
    Config();
};

static const relleums::Function::Func1 zero_efficiency(
    {
        {200e-9, 0},
        {1200e-9, 0}
    });

class Converter {
    const Config* config;

 public:
    explicit Converter(const Config* config);
    std::vector<ElectricPulse> get_pulse_pipeline_for_photon_pipeline(
        const std::vector<PipelinePhoton> &photon_pipeline,
        const double exposure_time,
        relleums::Random::Generator* prng);
    void add_pulse(
        const ElectricPulse &pulse,
        std::vector<ElectricPulse> *electric_pipeline,
        relleums::Random::Generator* prng)const;
    void add_accidental_pulse(
        std::vector<ElectricPulse> *electric_pipeline,
        const double exposure_time,
        relleums::Random::Generator* prng)const;
};

}  // namespace PhotoElectricConverter
}  // namespace SignalProcessing

#endif  // MCTRACER_SIGNALPROCESSING_PHOTOELECTRICCONVERTER_H_
