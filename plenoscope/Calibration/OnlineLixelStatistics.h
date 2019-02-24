// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_PLENOSCOPE_CALIBRATION_ONLINELIXELSTATISTICS_H_
#define MCTRACER_PLENOSCOPE_CALIBRATION_ONLINELIXELSTATISTICS_H_

#include "./OnlineStatistics.h"

namespace plenoscope {
namespace Calibration {

struct OnlineLixelStatistics {
    unsigned int count;
    OnlineStatistics cx;
    OnlineStatistics cy;
    OnlineStatistics x;
    OnlineStatistics y;
    OnlineStatistics timed_delay;
    OnlineLixelStatistics();
};

}  // namespace Calibration
}  // namespace plenoscope

#endif  // MCTRACER_PLENOSCOPE_CALIBRATION_ONLINELIXELSTATISTICS_H_
