//=================================
// include guard
#ifndef __PhotoElectricConverter_H_INCLUDED__
#define __PhotoElectricConverter_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include "Core/Function/Function.h"
#include "Core/Random/Random.h"
#include "PipelinePhoton.h"
#include "ElectricPulse.h"
//=================================
namespace SignalProcessing {
    namespace PhotoElectricConverter {

struct Config {

    const Function::Func1D* quantum_efficiency_vs_wavelength;
    double dark_rate;
    double probability_for_second_puls;
    Config();
};

static const Function::Constant perfect_efficiency(
    1.0, 
    Function::Limits(200e-9, 1200e-9)
);

class Converter {

    const Config* config;
public:

    Converter(const Config* config);

    vector<ElectricPulse> get_pulse_pipeline_for_photon_pipeline(
        const vector<PipelinePhoton> &photon_pipeline,
        const double exposure_time,
        Random::Generator* prng
    );

    void add_pulse(
        const ElectricPulse &pulse,
        vector<ElectricPulse> *electric_pipeline, 
        Random::Generator* prng
    )const;

    void add_accidental_pulse(
        vector<ElectricPulse> *electric_pipeline, 
        const double exposure_time,
        Random::Generator* prng
    )const;
};

    }//PhotoElectricConverter
}//SignalProcessing
#endif // __PhotoElectricConverter_H_INCLUDED__ 