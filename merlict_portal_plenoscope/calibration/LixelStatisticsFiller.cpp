// Copyright 2014 Sebastian A. Mueller
#include "merlict_portal_plenoscope/calibration/LixelStatisticsFiller.h"
#include <math.h>


namespace plenoscope {
namespace calibration {

LixelStatisticsFiller::LixelStatisticsFiller(
    const uint64_t num_lixel,
    const uint64_t num_blocks,
    const uint64_t num_photons_per_block):
    photons_emitted_per_lixel(
        static_cast<double>(num_blocks)*
        static_cast<double>(num_photons_per_block)/
        static_cast<double>(num_lixel)) {
    lixel_stats.resize(num_lixel);
}

void LixelStatisticsFiller::fill_in_block(
    const std::vector<CalibrationPhotonResult> &calib_block
) {
    for (CalibrationPhotonResult result : calib_block) {
        if (result.reached_sensor == true) {
            lixel_stats[result.lixel_id].count++;
            lixel_stats[result.lixel_id].cx.add(
                result.x_tilt_vs_optical_axis);
            lixel_stats[result.lixel_id].cy.add(
                result.y_tilt_vs_optical_axis);
            lixel_stats[result.lixel_id].x.add(
                result.x_pos_on_principal_aperture);
            lixel_stats[result.lixel_id].y.add(
                result.y_pos_on_principal_aperture);
            lixel_stats[result.lixel_id].timed_delay.add(
                result.time_of_flight);
        }
    }
}

std::vector<LixelStatistic> LixelStatisticsFiller::get_lixel_statistics()const {
    std::vector<LixelStatistic> lixel_statistics;
    lixel_statistics.reserve(lixel_stats.size());

    for (const OnlineLixelStatistics &lixel : lixel_stats) {
        LixelStatistic stat;
        stat.efficiency     = lixel.count/photons_emitted_per_lixel;
        stat.efficiency_std = sqrt(
            static_cast<float>(lixel.count))/photons_emitted_per_lixel;
        stat.cx_mean    = lixel.cx.mean();
        stat.cx_std     = lixel.cx.stddev();
        stat.cy_mean    = lixel.cy.mean();
        stat.cy_std     = lixel.cy.stddev();
        stat.x_mean     = lixel.x.mean();
        stat.x_std      = lixel.x.stddev();
        stat.y_mean     = lixel.y.mean();
        stat.y_std      = lixel.y.stddev();
        stat.time_delay_mean  = lixel.timed_delay.mean();
        stat.time_delay_std   = lixel.timed_delay.stddev();
        lixel_statistics.push_back(stat);
    }
    return lixel_statistics;
}

}  // namespace calibration
}  // namespace plenoscope
