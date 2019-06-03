// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_CALIBRATION_LIXELSTATISTICSFILLER_H_
#define PLENOSCOPE_CALIBRATION_LIXELSTATISTICSFILLER_H_

#include <vector>
#include "merlict_portal_plenoscope/calibration/CalibrationPhotonResult.h"
#include "merlict_portal_plenoscope/calibration/Config.h"
#include "merlict_portal_plenoscope/light_field_sensor/Geometry.h"
#include "merlict_portal_plenoscope/calibration/OnlineLixelStatistics.h"
#include "merlict_portal_plenoscope/calibration/LixelStatistics.h"

namespace plenoscope {
namespace calibration {

class LixelStatisticsFiller {
    std::vector<OnlineLixelStatistics> lixel_stats;
 public:
    const double photons_emitted_per_lixel;
    LixelStatisticsFiller(
        const uint64_t num_lixel,
        const uint64_t num_blocks,
        const uint64_t num_photons_per_block);
    std::vector<LixelStatistic> get_lixel_statistics()const;
    void fill_in_block(const std::vector<CalibrationPhotonResult> &calib_block);
 private:
    double min_arrival_time_mean()const;
};

}  // namespace calibration
}  // namespace plenoscope

#endif  // PLENOSCOPE_CALIBRATION_LIXELSTATISTICSFILLER_H_
