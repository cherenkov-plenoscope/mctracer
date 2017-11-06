//=================================
// include guard
#ifndef __SimpleTdcQdc_H_INCLUDED__
#define __SimpleTdcQdc_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <vector>
#include <string>
#include "ElectricPulse.h"

namespace SignalProcessing {
    namespace SimpleTdcQdc{

    struct TimeAndCount {
        double time;
        unsigned int count;
        std::vector<int> simulation_truth_ids;
        TimeAndCount(
            const double time,
            const unsigned int count,
            const std::vector<int> simulation_truth_ids
        );
    };

    std::vector<unsigned int> convolution_of_arrival_times_and_integration_window(
    	const std::vector<ElectricPulse> &arrival_moments,
        const double integration_time_window
    );

    unsigned int arrival_count_in_ith_integration_window(
    	const unsigned int i, 
    	const double integration_time, 
    	const std::vector<ElectricPulse> &arrival_pipeline
    );

    TimeAndCount get_arrival_time_and_count_given_arrival_moments_and_integration_time_window(
        const std::vector<ElectricPulse> &arrival_moments,
        const double integration_time_window
    );

    std::vector<int> simulation_truth_ids_in_time_window(
        const std::vector<ElectricPulse> &arrival_moments,
        const unsigned int start_pulse_index, 
        const double integration_time_window
    );

    void write_intensity_simulation_truth(
        const std::vector<TimeAndCount> &tacs,
        const std::string &path
    );
    
    }//SimpleTdcQdc
}//SignalProcessing
#endif // __SimpleTdcQdc_H_INCLUDED__