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
using std::vector;

namespace SimpleTdcQdc{

    struct TimeAndCount {
        double time;
        uint count;
    };

    vector<uint> convolution_of_arrival_times_and_integration_window(
    	const vector<double> &arrival_moments,
        const double integration_time_window
    );

    uint arrival_count_in_ith_integration_window(
    	const uint i, 
    	const double integration_time, 
    	const vector<double> &arrival_pipeline
    );

    TimeAndCount get_arrival_time_and_count_given_arrival_moments_and_integration_time_window(
        const vector<double> &arrival_moments,
        const double integration_time_window
    );
}//TdC QdC
#endif // __SimpleTdcQdc_H_INCLUDED__