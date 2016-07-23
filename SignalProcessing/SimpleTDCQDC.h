//=================================
// include guard
#ifndef __SimpleTdcQdc_H_INCLUDED__
#define __SimpleTdcQdc_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <vector>
#include <string>
using std::string;
using std::vector;
#include "ElectricPulse.h"

namespace SimpleTdcQdc{

    struct TimeAndCount {
        double time;
        uint count;
        vector<int> simulation_truth_ids;
        TimeAndCount(
            const double time,
            const uint count,
            const vector<int> simulation_truth_ids
        );
    };

    vector<uint> convolution_of_arrival_times_and_integration_window(
    	const vector<ElectricPulse> &arrival_moments,
        const double integration_time_window
    );

    uint arrival_count_in_ith_integration_window(
    	const uint i, 
    	const double integration_time, 
    	const vector<ElectricPulse> &arrival_pipeline
    );

    TimeAndCount get_arrival_time_and_count_given_arrival_moments_and_integration_time_window(
        const vector<ElectricPulse> &arrival_moments,
        const double integration_time_window
    );

    vector<int> simulation_truth_ids_in_time_window(
        const vector<ElectricPulse> &arrival_moments,
        const uint start_pulse_index, 
        const double integration_time_window
    );

    void write_intensity_simulation_truth(
        const vector<TimeAndCount> &tacs,
        const string &path
    );
}//TdC QdC
#endif // __SimpleTdcQdc_H_INCLUDED__