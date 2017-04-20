//=================================
// include guard
#ifndef __PhotonStream_H_INCLUDED__
#define __PhotonStream_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <vector>
#include <string>
#include "SignalProcessing/ElectricPulse.h"
using std::vector;
using std::string;

namespace SignalProcessing {
    namespace PhotonStream {
// Write and read ElectricPulses in the very compact photon_stream format.

const unsigned char NEXT_READOUT_CHANNEL_MARKER = 255;

void write(
    const vector<vector<SignalProcessing::ElectricPulse>> &pulses,
    const float slice_duration,
    const string path
);

void write_simulation_truth(
    const vector<vector<SignalProcessing::ElectricPulse>> &pulses,
    const string path
);

struct Stream {
    vector<vector<SignalProcessing::ElectricPulse>> photon_stream;
    float slice_duration;
    Stream();
};

Stream read(const string path);

Stream read_with_simulation_truth(const string path, const string truth_path);

uint number_of_pulses(
    const vector<vector<SignalProcessing::ElectricPulse>> &pulses
);

uint number_of_symbols_to_represent_pulses(
    const vector<vector<SignalProcessing::ElectricPulse>> &pulses
);

void assert_number_time_slices_below_8bit_max(
    const uint32_t number_time_slices);

    }//PhotonStream
}//SignalProcessing
#endif // __PhotonStream_H_INCLUDED__

