#include "SimpleTDCQDC.h"
#include <iostream>
#include <fstream>
#include <sstream>
using std::string;
using std::vector;

namespace SignalProcessing {
    namespace SimpleTdcQdc{
//------------------------------------------------------------------------------
TimeAndCount::TimeAndCount(
    const double _time,
    const unsigned int _count,
    const vector<int> _simulation_truth_ids
):
    time(_time),
    count(_count),
    simulation_truth_ids(_simulation_truth_ids)
{}
//------------------------------------------------------------------------------
TimeAndCount get_arrival_time_and_count_given_arrival_moments_and_integration_time_window(
    const vector<ElectricPulse> &arrival_moments,
    const double integration_time_window
) { 
    vector<unsigned int> enclo = convolution_of_arrival_times_and_integration_window(
        arrival_moments,
        integration_time_window
    );

    if(enclo.size() == 0) 
        return TimeAndCount(0.0, 0u, vector<int>());

    unsigned int max = 0;
    unsigned int max_idx = 0;
    for(unsigned int i=0; i<enclo.size(); i++) {
        if(enclo[i]>max) {
            max = enclo[i];
            max_idx = i;
        }
    }

    return TimeAndCount(
        arrival_moments[max_idx].arrival_time,
        max,
        simulation_truth_ids_in_time_window(
            arrival_moments,
            max_idx,
            integration_time_window
        )
    );
}
//------------------------------------------------------------------------------
vector<unsigned int> convolution_of_arrival_times_and_integration_window(
    const vector<ElectricPulse> &arrival_moments,
    const double integration_time_window
) {
    vector<unsigned int> enclos;
    enclos.reserve(arrival_moments.size());

    for(unsigned int i=0; i<arrival_moments.size(); i++)
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
unsigned int arrival_count_in_ith_integration_window(
    const unsigned int i,
    const double integration_time,
    const vector<ElectricPulse> &arrival_moments
) {
    unsigned int arrival_count = 1;
    while(
        i+arrival_count < arrival_moments.size() &&
        arrival_moments[i+arrival_count].arrival_time - 
        arrival_moments[i].arrival_time < 
        integration_time
    ) {
        arrival_count++;
    }
    return arrival_count;
}
//------------------------------------------------------------------------------
vector<int> simulation_truth_ids_in_time_window(
    const vector<ElectricPulse> &arrival_moments,
    const unsigned int start_pulse_index,
    const double integration_time_window
) {
    vector<int> simulation_truth_ids;

    const double start_time = arrival_moments[start_pulse_index].arrival_time;

    for(unsigned int idx = start_pulse_index; idx<arrival_moments.size(); idx++) {
        if(
            arrival_moments[idx].arrival_time - start_time > integration_time_window
        ) {
            break;
        }else{
            simulation_truth_ids.push_back(arrival_moments[idx].simulation_truth_id);
        }
    }

    return simulation_truth_ids;
}
//------------------------------------------------------------------------------
void write_intensity_simulation_truth(
    const vector<TimeAndCount> &tacs,
    const string &path
) {
    std::ofstream file (path.c_str());
    if(file.is_open()) {

        for(unsigned int i=0; i<tacs.size(); i++) {
            for(unsigned int j=0; j<tacs[i].simulation_truth_ids.size(); j++) {
                file << tacs[i].simulation_truth_ids[j];
                if(j+1 < tacs[i].simulation_truth_ids.size()) {
                   file << " ";
                }
            }
            file << "\n";
        }

        file.close();
    }else{
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "Can not write file '" << path << "'.";
        throw std::runtime_error(info.str());
    } 
}
//------------------------------------------------------------------------------
    }//TdC QdC
}//SignalProcessing