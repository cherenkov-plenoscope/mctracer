// Copyright 2015 Sebastian A. Mueller
#ifndef MCTRACER_SIGNALPROCESSING_TIMELINEADDITION_H_
#define MCTRACER_SIGNALPROCESSING_TIMELINEADDITION_H_

#include <vector>

namespace SignalProcessing {

void add_second_to_first_at(
    std::vector<double> *first,
    const std::vector<double> *second,
    int i);

}  // namespace SignalProcessing

#endif  // MCTRACER_SIGNALPROCESSING_TIMELINEADDITION_H_
