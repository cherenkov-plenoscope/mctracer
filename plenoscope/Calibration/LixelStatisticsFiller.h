// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_CALIBRATION_LIXELSTATISTICSFILLER_H_
#define PLENOSCOPE_CALIBRATION_LIXELSTATISTICSFILLER_H_

#include <vector>
#include "plenoscope/Calibration/CalibrationPhotonResult.h"
#include "plenoscope/Calibration/Config.h"
#include "plenoscope/LightFieldSensor/Geometry.h"
#include "plenoscope/Calibration/OnlineLixelStatistics.h"
#include "plenoscope/Calibration/LixelStatistics.h"

namespace plenoscope {
namespace Calibration {

class LixelStatisticsFiller {
    const Config *calib_config;
    const LightFieldSensor::Geometry *sensor_geometry;
    std::vector<OnlineLixelStatistics> lixel_stats;
 public:
    const double photons_emitted_per_lixel;
    LixelStatisticsFiller(
        const LightFieldSensor::Geometry *sensor_geometry,
        const Config *config);
    std::vector<LixelStatistic> get_lixel_statistics()const;
    void fill_in_block(const std::vector<CalibrationPhotonResult> &calib_block);
 private:
    double min_arrival_time_mean()const;
};

}  // namespace Calibration
}  // namespace plenoscope

#endif  // PLENOSCOPE_CALIBRATION_LIXELSTATISTICSFILLER_H_
