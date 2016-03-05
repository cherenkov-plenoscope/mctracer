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
std::vector<double> Converter::get_pulse_pipeline_for_photon_pipeline(
	const std::vector<PipelinePhoton> &photon_pipeline,
	const double exposure_time,
	Random::Generator* prng
) {

	std::vector<double> electric_pipeline;

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
	std::vector<double> *electric_pipeline, 
	Random::Generator* prng
)const {

	if(config->probability_for_second_puls >= prng->uniform()) {
		electric_pipeline->push_back(arrival_time);
		add_time_to_electric_pipeline(arrival_time, electric_pipeline, prng);
	}else{
		electric_pipeline->push_back(arrival_time);
	}
}
//------------------------------------------------------------------------------
void Converter::add_dark_rate(
	std::vector<double> *electric_pipeline, 
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
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
SamplerConfig::SamplerConfig() {

	exposure_time = 150e-9;
	sampling_frequency = 2e9;
	std_dev_noise = 0.0;
	pulse_template = &box_pulse_5ns;
}
//------------------------------------------------------------------------------
Sampler::Sampler(const SamplerConfig* config) {

	this->config = config;

	if(config->std_dev_noise < 0.0) {
		std::stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected config->std_dev_noise > 0.0, ";
		info << "but actual it is: " << config->std_dev_noise;
		info << "\n";
		throw TracerException(info.str());
	}

	if(config->sampling_frequency < 0.0) {
		std::stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected config->sampling_frequency > 0.0, ";
		info << "but actual it is: " << config->sampling_frequency;
		info << "\n";
		throw TracerException(info.str());
	}

	if(config->exposure_time < 0.0) {
		std::stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected config->exposure_time > 0.0, ";
		info << "but actual it is: " << config->exposure_time;
		info << "\n";
		throw TracerException(info.str());
	}

	number_of_slices = uint(ceil(
		config->exposure_time*config->sampling_frequency
	));
}
//------------------------------------------------------------------------------
std::vector<double> Sampler::time_line(
	const std::vector<double> *electric_pipeline,
	const Function::DiscretSampling::LookUpTable* puls_look_up_table,
	Random::Generator* prng
) {	
	std::vector<double> time_line;
	time_line.reserve(number_of_slices);

	// add noise
	for(uint i=0; i<number_of_slices; i++)
		time_line.push_back(prng->normal(0.0, config->std_dev_noise));

	// add pulses
	for(uint p=0; p<electric_pipeline->size(); p++) {

		const double start_pos_in_slices = 
			electric_pipeline->at(p)*config->sampling_frequency;

		const uint start_slice = floor(start_pos_in_slices);
		
		const double start_offset_in_slices = fmod(start_pos_in_slices, 1);

		const std::vector<double> *pulse = 
			puls_look_up_table->at(start_offset_in_slices);

		add_second_to_first_at(&time_line, pulse, start_slice);
	}

	return time_line;
}
//------------------------------------------------------------------------------
uint Sampler::get_number_of_slices()const {
	return number_of_slices;
}
//------------------------------------------------------------------------------
}// PhotoElectricConverter