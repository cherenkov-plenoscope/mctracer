// Copyright 2016 Sebastian A. Mueller
#ifndef PLENOSCOPE_NIGHTSKYBACKGROUND_INJECTOR_H_
#define PLENOSCOPE_NIGHTSKYBACKGROUND_INJECTOR_H_

#include <vector>
#include "plenoscope/NightSkyBackground/Light.h"
#include "signal_processing/signal_processing.h"
#include "plenoscope/Calibration/LixelStatistics.h"

namespace plenoscope {
namespace NightSkyBackground {

void inject_nsb_into_photon_pipeline(
    std::vector<std::vector<signal_processing::PipelinePhoton>> *
        photon_pipelines,
    const double exposure_time,
    const std::vector<plenoscope::Calibration::LixelStatistic> *
        lixel_statistics,
    const Light *nsb,
    relleums::random::Generator* prng
);

}  // namespace NightSkyBackground
}  // namespace plenoscope

#endif  // PLENOSCOPE_NIGHTSKYBACKGROUND_INJECTOR_H_
