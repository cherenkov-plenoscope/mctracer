// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_CALIBRATION_LIXELSTATISTICS_H_
#define PLENOSCOPE_CALIBRATION_LIXELSTATISTICS_H_

#include <string>
#include <vector>
#include "merlict_portal_plenoscope/calibration/CalibrationPhotonResult.h"
#include "merlict_portal_plenoscope/calibration/Config.h"
#include "merlict_portal_plenoscope/light_field_sensor/Geometry.h"
#include "merlict_portal_plenoscope/calibration/OnlineLixelStatistics.h"

namespace plenoscope {
namespace calibration {

//--lixel_statistics_s--
struct LixelStatistic {
    float efficiency, efficiency_std;
    float cx_mean, cx_std;
    float cy_mean, cy_std;
    float x_mean, x_std;
    float y_mean, y_std;
    float time_delay_mean, time_delay_std;
    LixelStatistic();
};
//--lixel_statistics_e--

void write(
    const std::vector<LixelStatistic> &lixel_statistics,
    const std::string &path);
std::vector<LixelStatistic> read(const std::string &path);

}  // namespace calibration
}  // namespace plenoscope

#endif  // PLENOSCOPE_CALIBRATION_LIXELSTATISTICS_H_
