// Copyright 2017 Sebastian A. Mueller
#include <math.h>
#include "signal_processing/pulse_extraction.h"
#include "merlict/merlict.h"
using std::vector;

namespace signal_processing {

vector<ExtractedPulse> extract_pulses(
    const vector<ElectricPulse> &electric_pulses,
    const double time_slice_duration,
    const double arrival_time_std,
    relleums::random::Generator* prng
) {
    vector<ExtractedPulse> channel;
    channel.reserve(electric_pulses.size());
    for (uint32_t p = 0; p < electric_pulses.size(); p++) {
        const double true_arrival_time = electric_pulses.at(p).arrival_time;
        const double reconstructed_arrival_time = true_arrival_time +
            prng->normal(0.0f, arrival_time_std);
        const int32_t slice = round(
            reconstructed_arrival_time/time_slice_duration);
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
    const double time_slice_duration,
    const double arrival_time_std,
    relleums::random::Generator* prng
) {
    vector<vector<ExtractedPulse>> channels;
    channels.reserve(electric_pulses.size());
    for (uint32_t channel = 0; channel < electric_pulses.size(); channel++) {
        channels.emplace_back(
            extract_pulses(
                electric_pulses.at(channel),
                time_slice_duration,
                arrival_time_std,
                prng));
    }
    return channels;
}

}  // namespace signal_processing
