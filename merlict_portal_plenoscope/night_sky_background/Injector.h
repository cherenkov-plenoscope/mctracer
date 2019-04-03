// Copyright 2016 Sebastian A. Mueller
#ifndef PLENOSCOPE_NIGHTSKYBACKGROUND_INJECTOR_H_
#define PLENOSCOPE_NIGHTSKYBACKGROUND_INJECTOR_H_

#include <vector>
#include "merlict_portal_plenoscope/night_sky_background/Light.h"
#include "merlict_signal_processing/signal_processing.h"
#include "merlict_portal_plenoscope/calibration/LixelStatistics.h"

namespace plenoscope {
namespace night_sky_background {

void inject_nsb_into_photon_pipeline(
    std::vector<std::vector<signal_processing::PipelinePhoton>> *
        photon_pipelines,
    const double exposure_time,
    const std::vector<plenoscope::calibration::LixelStatistic> *
        lixel_statistics,
    const Light *nsb,
    merlict::random::Generator* prng
);

}  // namespace night_sky_background
}  // namespace plenoscope

#endif  // PLENOSCOPE_NIGHTSKYBACKGROUND_INJECTOR_H_
