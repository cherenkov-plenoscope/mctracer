// Copyright 2014 Sebastian A. Mueller
#include <vector>
#include "catch.hpp"
#include "signal_processing/PhotoElectricConverter.h"
#include "Core/Histogram1.h"
using std::vector;
using namespace relleums;



vector<signal_processing::PipelinePhoton> equi_distant_photons(
    const unsigned int n
) {
    vector<signal_processing::PipelinePhoton> photon_pipeline;
    for (unsigned int i = 0; i < n; i++) {
        const double arrival_time = static_cast<double>(i)*1e-9;
        const double wavelength = 433e-9;
        const int simulation_truth_id = i;

        photon_pipeline.push_back(
            signal_processing::PipelinePhoton(
                arrival_time,
                wavelength,
                simulation_truth_id));
    }
    return photon_pipeline;
}


TEST_CASE("PhotoElectricConverterTest: config_defaults", "[mctracer]") {
    signal_processing::PhotoElectricConverter::Config config;
    CHECK(config.dark_rate == 0.0);
    CHECK(config.probability_for_second_puls == 0.0);

    for (double wavelength = 200e-9; wavelength < 1200e-9; wavelength+=10e-9)
        CHECK(config.quantum_efficiency_vs_wavelength->evaluate(wavelength) == 0.0);

    CHECK_THROWS_AS(config.quantum_efficiency_vs_wavelength->evaluate(1200e-9), std::out_of_range);
    CHECK_THROWS_AS(config.quantum_efficiency_vs_wavelength->evaluate(199e-9), std::out_of_range);
}

TEST_CASE("PhotoElectricConverterTest: empty_input_yields_empty_output", "[mctracer]") {
    signal_processing::PhotoElectricConverter::Config config;
    signal_processing::PhotoElectricConverter::Converter conv(&config);

    vector<signal_processing::PipelinePhoton> photon_pipeline;
    double exposure_time = 1.0;
    random::Mt19937 prng(0);
    vector<signal_processing::ElectricPulse> result = conv.
        get_pulse_pipeline_for_photon_pipeline(
            photon_pipeline,
            exposure_time,
            &prng);

    CHECK(result.size() == 0u);
}

TEST_CASE("PhotoElectricConverterTest: input_pulses_absorbed_zero_qunatum_eff", "[mctracer]") {
    signal_processing::PhotoElectricConverter::Config config;
    signal_processing::PhotoElectricConverter::Converter conv(&config);

    vector<signal_processing::PipelinePhoton> photon_pipeline =
        equi_distant_photons(1337u);

    double exposure_time = 1337e-9;
    random::Mt19937 prng(0);
    vector<signal_processing::ElectricPulse> result = conv.
        get_pulse_pipeline_for_photon_pipeline(
            photon_pipeline,
            exposure_time,
            &prng);

    CHECK(result.size() == 0u);
}

TEST_CASE("PhotoElectricConverterTest: input_pulses_pass_qunatum_eff_is_one", "[mctracer]") {
    signal_processing::PhotoElectricConverter::Config config;
    function::Func1 qeff({{200e-9, 1.0}, {1200e-9, 1.0}});
    config.quantum_efficiency_vs_wavelength = &qeff;
    signal_processing::PhotoElectricConverter::Converter conv(&config);

    vector<signal_processing::PipelinePhoton> photon_pipeline =
        equi_distant_photons(1337u);

    double exposure_time = 1337e-9;
    random::Mt19937 prng(0);
    vector<signal_processing::ElectricPulse> result = conv.
        get_pulse_pipeline_for_photon_pipeline(
            photon_pipeline,
            exposure_time,
            &prng);

    REQUIRE(result.size() == 1337u);
    for (unsigned int i = 0; i < result.size(); i++) {
        CHECK(result.at(i).arrival_time == photon_pipeline.at(i).arrival_time);
    }
}

TEST_CASE("PhotoElectricConverterTest: dark_rate_on_empty_photon_pipe", "[mctracer]") {
    signal_processing::PhotoElectricConverter::Config config;
    config.dark_rate = 100e6;
    signal_processing::PhotoElectricConverter::Converter conv(&config);

    vector<signal_processing::PipelinePhoton> photon_pipeline;

    double exposure_time = 1e-6;
    random::Mt19937 prng(0);
    vector<signal_processing::ElectricPulse> result = conv.
        get_pulse_pipeline_for_photon_pipeline(
            photon_pipeline,
            exposure_time,
            &prng);

    CHECK(100u == Approx(result.size()).margin(3));
}

TEST_CASE("PhotoElectricConverterTest: triangle_qeff", "[mctracer]") {
    // Qeff [1]                                            //
    //                                                     //
    // _1.0_|                                              //
    //      |            /\                                //
    //      |           /  \                               //
    //      |          /    \                              //
    //      |         /      \                             //
    //      |        /        \                            //
    //      |       /          \                           //
    //      |      /            \                          //
    //      |     /              \                         //
    //      |    /                \                        //
    //      |   /                  \                       //
    //      |  /                    \                      //
    // _0.0_|--|----------|----------|----                 //
    //        200e-9    700e-9     1200e-9 [nm] wavelength //

    signal_processing::PhotoElectricConverter::Config config;
    vector<vector<double>> raw_qeff {{  200e-9, 0.0},
                                     {  700e-9, 1.0},
                                     { 1200e-9, 0.0}};
    function::Func1 qeff(raw_qeff);
    config.quantum_efficiency_vs_wavelength = &qeff;
    signal_processing::PhotoElectricConverter::Converter conv(&config);

    vector<signal_processing::PipelinePhoton> photon_pipeline;
    const unsigned int n = 1000*1000;
    for (unsigned int i = 0; i < n; i++) {
        const double arrival_time = static_cast<double>(i);
        const double wavelength = 200e-9 + static_cast<double>(i)/
            static_cast<double>(n)*1000e-9;
        const int simulation_truth_id = i;
        photon_pipeline.push_back(
            signal_processing::PipelinePhoton(
                arrival_time,
                wavelength,
                simulation_truth_id));
    }

    double exposure_time = 1.0;
    random::Mt19937 prng(0);
    vector<signal_processing::ElectricPulse> result = conv.
        get_pulse_pipeline_for_photon_pipeline(
            photon_pipeline,
            exposure_time,
            &prng);

    CHECK(n/2 == Approx(result.size()).margin(static_cast<double>(n)));

    vector<double> survived_arrival_times;
    // We created a correlation between wavelength and arrival
    // time in this case on purpose.
    for (const signal_processing::ElectricPulse pulse : result)
        survived_arrival_times.push_back(pulse.arrival_time);

    Histogram1 hist(
        survived_arrival_times,
        {
            0,
            100*1000,
            200*1000,
            300*1000,
            400*1000,
            500*1000,
            600*1000,
            700*1000,
            800*1000,
            900*1000,
            1000*1000
        });

    CHECK(10*1000u == Approx(hist.bins.at(0)).margin(58));
    CHECK(30*1000u == Approx(hist.bins.at(1)).margin(253));
    CHECK(50*1000u == Approx(hist.bins.at(2)).margin(119));
    CHECK(70*1000u == Approx(hist.bins.at(3)).margin(39));
    CHECK(90*1000u == Approx(hist.bins.at(4)).margin(54));
    CHECK(90*1000u == Approx(hist.bins.at(5)).margin(228));
    CHECK(70*1000u == Approx(hist.bins.at(6)).margin(71));
    CHECK(50*1000u == Approx(hist.bins.at(7)).margin(194));
    CHECK(30*1000u == Approx(hist.bins.at(8)).margin(37));
    CHECK(10*1000u == Approx(hist.bins.at(9)).margin(25));
}
