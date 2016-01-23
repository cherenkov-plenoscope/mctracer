#include "NightSkyBackgroundLightInjector.h"
#include "Tools/Tools.h"
#include "Core/Histogram1D.h"

namespace LightFieldTelescope {

	void inject_nsb_into_photon_pipeline(
		std::vector<std::vector<PipelinePhoton>> *photon_pipelines,
		const double exposure_time,
		const std::vector<std::vector<double>> *lft_calib_result,
		const NightSkyBackgroundLight *telescope_nsb,
		Random::Generator* prng
	) {	
		//std::cout << __LINE__ << "\n";

		if(photon_pipelines->size() == 0)
			return;

		const double number_subpix = 
			telescope_nsb->telescope_geometry->total_number_of_sub_pixels();

		//---------------------------
		// FIND MIN MAX ARRIVAL TIMES

		uint number_cherenkov_photons = 0;
		std::vector<double> arrival_times;
		double min_arrival_time = std::numeric_limits<double>::max();
		double max_arrival_time = std::numeric_limits<double>::min();

		// for each sub_pixel
		for(uint i=0; i<photon_pipelines->size(); i++) {
				
			if(photon_pipelines->at(i).size() > 0) {
				if(photon_pipelines->at(i).front().arrival_time < min_arrival_time)
					min_arrival_time = photon_pipelines->at(i).front().arrival_time;
			
				if(photon_pipelines->at(i).back().arrival_time > max_arrival_time)
					max_arrival_time = photon_pipelines->at(i).back().arrival_time;

				number_cherenkov_photons += photon_pipelines->at(i).size();
			}

			for(PipelinePhoton ph: photon_pipelines->at(i))
				arrival_times.push_back(ph.arrival_time);
		}

		//---------------------------
		// FIND MODE IN ARRIVAL TIMES

		double mode_of_cherenkov_arrival_times;

		if(number_cherenkov_photons == 0) {

			mode_of_cherenkov_arrival_times = 0.0;
		}else{

			const uint bin_count = sqrt(number_cherenkov_photons);

			std::vector<double> arrival_time_bin_edges = numeric::linspace(
				min_arrival_time, 
				max_arrival_time, 
				bin_count
			);

			Histogram1D arrival_time_histo(arrival_times, arrival_time_bin_edges);

			mode_of_cherenkov_arrival_times = arrival_time_histo.mode();
		}

		//std::cout << "min: " << min_arrival_time << ", max: " << max_arrival_time  << ", duration: " << max_arrival_time - min_arrival_time << ", mode " << mode_of_cherenkov_arrival_times << "\n";

		//--------------------------------
		// INIT START TIME OF NSB EXPOSURE

		double start_time = mode_of_cherenkov_arrival_times - 0.5*exposure_time;

		// for each sub_pixel
		for(uint i=0; i<photon_pipelines->size(); i++) {

			double subpix_efficincy = lft_calib_result->at(i).at(0);

			double subpix_nsb_rate = 
				telescope_nsb->get_overall_rate()*subpix_efficincy/number_subpix;

			// arrival times of nsb
			std::vector<double> nsb_arrival_times;
			double relative_arrival_times_sum = prng->expovariate(subpix_nsb_rate);
			while(relative_arrival_times_sum < exposure_time) {

				double time_until_next_photon = prng->expovariate(subpix_nsb_rate);

				nsb_arrival_times.push_back(start_time + relative_arrival_times_sum);
				relative_arrival_times_sum += time_until_next_photon;
			};

			for(double nsb_arrival_time: nsb_arrival_times) {
				PipelinePhoton nsb_ph;
				nsb_ph.arrival_time = nsb_arrival_time;
				nsb_ph.wavelength = telescope_nsb->draw_wavelength(prng);

				photon_pipelines->at(i).push_back(nsb_ph);
			}

			sort_photon_pipelines_arrival_time(&photon_pipelines->at(i));
		}

		// substract global time offset
		if(number_cherenkov_photons > 0) {

			// for each sub_pixel
			for(uint i=0; i<photon_pipelines->size(); i++) {

				// for each photon
				for(uint p=0; p<photon_pipelines->at(i).size(); p++) {

					photon_pipelines->at(i).at(p).arrival_time -= min_arrival_time;
				}
			}
		}
	}
}

