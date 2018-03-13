// Copyright 2015 Sebastian A. Mueller
#ifndef MCTRACER_SIGNALPROCESSING_PRETRIGGER_H_
#define MCTRACER_SIGNALPROCESSING_PRETRIGGER_H_

#include <vector>

namespace SignalProcessing {
namespace PreTrigger {

struct Config {
    double time_window;
    unsigned int threshold;
    Config();
};

bool might_trigger(
    const std::vector<double> *arrival_moments,
    const Config &config
);

}  // namespace PreTrigger
}  // namespace SignalProcessing

#endif  // MCTRACER_SIGNALPROCESSING_PRETRIGGER_H_
