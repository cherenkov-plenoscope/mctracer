#include "PreTrigger.h"
//------------------------------------------------------------------------------
namespace PreTrigger {

	Config::Config() {
		time_window = 1.0;
		threshold = 1;
	}
	//--------------------------------------------------------------------------
	bool might_trigger(
		const std::vector<double> *arrival_moments,
		const Config &config
	) {
		double growing_delay;
		uint j;	

		if(arrival_moments->size() == 0) {

			if(config.threshold == 0)

				return true;
			else

				return false;
		}else if(arrival_moments->size() == 1 && config.threshold <= 1) {

			if(config.threshold <= 1)

				return true;
			else

				return false;
		}else{

			for(uint i=0; i<arrival_moments->size()-1; i++) {
				
				growing_delay = 0.0;
				j = 1;

				while(growing_delay < config.time_window && j<arrival_moments->size()-i-1) {
					std::cout << "i: " << i << ", j: " << j << "\n";	
					if(j >= config.threshold)
						return true;
					
					growing_delay += arrival_moments->at(i+1+j) - arrival_moments->at(i+j);
					j++;
				}
			}

			return false;
		}
	}
	//--------------------------------------------------------------------------
}// PreTrigger
