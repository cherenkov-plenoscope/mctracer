#include "PhotoElectricConverter.h"


namespace PhotoElectricConverter {
//------------------------------------------------------------------------------
Config::Config() {
	dark_rate = 0.0;
	probability_for_second_puls = 0.0;
	quantum_efficiency_vs_wavelength = &perfect_efficiency;
}
//------------------------------------------------------------------------------
Converter::Converter(const Config* config) {
	this->config = config;

	if(config->probability_for_second_puls >= 1.0) {
		std::stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected config->probability_for_second_puls < 1.0, ";
		info << "but actual it is: " << config->probability_for_second_puls;
		info << "\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
vector<double> Converter::get_pulse_pipeline_for_photon_pipeline(
	const vector<PipelinePhoton> &photon_pipeline,
	const double exposure_time,
	Random::Generator* prng
) {

	vector<double> electric_pipeline;

	for(PipelinePhoton ph: photon_pipeline) {

		if((*config->quantum_efficiency_vs_wavelength)(ph.wavelength) 
			>= prng->uniform()) {
			add_time_to_electric_pipeline(ph.arrival_time, &electric_pipeline, prng);
		}
			
	}

	add_dark_rate(&electric_pipeline, exposure_time, prng);

	return electric_pipeline;
}
//------------------------------------------------------------------------------
void Converter::add_time_to_electric_pipeline(
	const double arrival_time,
	vector<double> *electric_pipeline, 
	Random::Generator* prng
)const {
	electric_pipeline->push_back(arrival_time);
	if(config->probability_for_second_puls >= prng->uniform())
		add_time_to_electric_pipeline(arrival_time, electric_pipeline, prng);
}
//------------------------------------------------------------------------------
void Converter::add_dark_rate(
	vector<double> *electric_pipeline, 
	const double exposure_time,
	Random::Generator* prng
)const{

	double relative_arrival_times_sum = prng->expovariate(config->dark_rate);
	while(relative_arrival_times_sum < exposure_time) {

		double time_until_next_pulse = prng->expovariate(config->dark_rate);

		add_time_to_electric_pipeline(
			relative_arrival_times_sum,
			electric_pipeline,
			prng
		);

		relative_arrival_times_sum += time_until_next_pulse;
	};
}
//------------------------------------------------------------------------------
}// PhotoElectricConverter