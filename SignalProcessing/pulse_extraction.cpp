// Copyright 2017 Sebastian A. Mueller
#include <math.h>
#include "SignalProcessing/pulse_extraction.h"
#include "Core/SimulationTruth.h"
using std::vector;

namespace SignalProcessing {

vector<ExtractedPulse> extract_pulses(
    const vector<ElectricPulse> &electric_pulses,
    const double time_slice_duration
) {
    vector<ExtractedPulse> channel;
    channel.reserve(electric_pulses.size());
    for (uint32_t p = 0; p < electric_pulses.size(); p++) {
        const int32_t slice = round(
            electric_pulses.at(p).arrival_time/time_slice_duration);
        if (slice >= 0 && slice < NUMBER_TIME_SLICES) {
            channel.emplace_back(
                ExtractedPulse(
                    static_cast<uint8_t>(slice),
                    electric_pulses.at(p).simulation_truth_id));
        }
    }
    return channel;
}

vector<vector<ExtractedPulse>> extract_pulses(
    const vector<vector<ElectricPulse>> &electric_pulses,
    const double time_slice_duration
) {
    vector<vector<ExtractedPulse>> channels;
    channels.reserve(electric_pulses.size());
    for (uint32_t channel = 0; channel < electric_pulses.size(); channel++) {
        channels.emplace_back(
            extract_pulses(
                electric_pulses.at(channel),
                time_slice_duration));
    }
    return channels;
}

}  // namespace SignalProcessing
