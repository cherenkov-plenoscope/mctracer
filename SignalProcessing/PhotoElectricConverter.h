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
//#include <random>
//=================================

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

		vector<double> get_pulse_pipeline_for_photon_pipeline(
			const vector<PipelinePhoton> &photon_pipeline,
			const double exposure_time,
			Random::Generator* prng
		);

		void add_time_to_electric_pipeline(
			const double arrival_time,
			vector<double> *electric_pipeline, 
			Random::Generator* prng
		)const;

		void add_dark_rate(
			vector<double> *electric_pipeline, 
			const double exposure_time,
			Random::Generator* prng
		)const;
	};

	struct SamplerConfig {
		
		double exposure_time;
		double sampling_frequency;
		double std_dev_noise;
		const Function::Func1D* pulse_template;
		SamplerConfig();
	};

	static const Function::Constant box_pulse_5ns(
		1.0, 
		Function::Limits(0.0e-9, 5.0e-9)
	);

	class Sampler {

		uint number_of_slices;
		const SamplerConfig* config;
	public:

		Sampler(const SamplerConfig* config);
		vector<double> time_line(
			const vector<double> *electric_pipeline,
			const Function::DiscretSampling::LookUpTable* puls_look_up_table,
			Random::Generator* prng
		);

		uint get_number_of_slices()const;
	};

}// PhotoElectricConverter
#endif // __PhotoElectricConverter_H_INCLUDED__ 