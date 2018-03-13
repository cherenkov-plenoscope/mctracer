// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_SIGNALPROCESSING_ELECTRICPULSE_H_
#define MCTRACER_SIGNALPROCESSING_ELECTRICPULSE_H_

namespace SignalProcessing {

struct ElectricPulse {
    double arrival_time;
    int simulation_truth_id;
    ElectricPulse();
    ElectricPulse(
        const double arrival_time,
        const int simulation_truth_id);
};

}  // namespace SignalProcessing

#endif  // MCTRACER_SIGNALPROCESSING_ELECTRICPULSE_H_
