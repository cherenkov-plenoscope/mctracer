#include "SimpleTDCQDC.h"

namespace SimpleTdcQdc{
//------------------------------------------------------------------------------
TimeAndCount get_arrival_time_and_count_given_arrival_moments_and_integration_time_window(
    const vector<double> &arrival_moments,
    const double integration_time_window
) {
    vector<uint> enclo = convolution_of_arrival_times_and_integration_window(
        arrival_moments,
        integration_time_window
    );

    if(enclo.size() == 0) {
        TimeAndCount tac;
        tac.time = 0.0;
        tac.count = 0.0;
        return tac;
    }

    uint max = 0;
    uint max_idx = 0;
    for(uint i=0; i<enclo.size(); i++) {
        if(enclo[i]>max) {
            max = enclo[i];
            max_idx = i;
        }
    }

    TimeAndCount tac;
    tac.time = arrival_moments[max_idx];
    tac.count = max;

    return tac;
}
//------------------------------------------------------------------------------
vector<uint> convolution_of_arrival_times_and_integration_window(
    const vector<double> &arrival_moments,
    const double integration_time_window
) {
    vector<uint> enclos;
    enclos.reserve(arrival_moments.size());

    for(uint i=0; i<arrival_moments.size(); i++)
        enclos.push_back(
            arrival_count_in_ith_integration_window(
                i, 
                integration_time_window, 
                arrival_moments
            )
        );
    
    return enclos;
}
//------------------------------------------------------------------------------
uint arrival_count_in_ith_integration_window(
    const uint i,
    const double integration_time,
    const vector<double> &arrival_moments
) {
    uint arrival_count = 1;
    while(
        i+arrival_count < arrival_moments.size() &&
        arrival_moments[i+arrival_count] - arrival_moments[i] < integration_time
    ) {
        arrival_count++;
    }
    return arrival_count;
}
//------------------------------------------------------------------------------
}//TdC QdC