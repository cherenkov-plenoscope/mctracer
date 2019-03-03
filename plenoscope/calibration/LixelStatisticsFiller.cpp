// Copyright 2014 Sebastian A. Mueller
#include "plenoscope/calibration/LixelStatisticsFiller.h"
#include <math.h>


namespace plenoscope {
namespace calibration {

LixelStatisticsFiller::LixelStatisticsFiller(
    const light_field_sensor::Geometry *geometry,
    const Config *_calib_config
):
    calib_config(_calib_config),
    sensor_geometry(geometry),
    photons_emitted_per_lixel(
        static_cast<double>(_calib_config->num_blocks)*
        static_cast<double>(_calib_config->photons_per_block)/
        static_cast<double>(geometry->num_lixel())) {
    lixel_stats.resize(sensor_geometry->num_lixel());
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
                result.relative_time_of_flight);
        }
    }
}

double LixelStatisticsFiller::min_arrival_time_mean()const {
    double minimal_time_mean = 0.0;
    if (lixel_stats.size() > 0)
        minimal_time_mean = lixel_stats.at(0).timed_delay.mean();
    for (OnlineLixelStatistics lixel : lixel_stats) {
        double this_pix_time = lixel.timed_delay.mean();
        if (this_pix_time < minimal_time_mean)
            minimal_time_mean = this_pix_time;
    }
    return minimal_time_mean;
}

std::vector<LixelStatistic> LixelStatisticsFiller::get_lixel_statistics()const {
    const double minimal_arrival_time = min_arrival_time_mean();

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
        stat.time_delay_mean  = lixel.timed_delay.mean() - minimal_arrival_time;
        stat.time_delay_std   = lixel.timed_delay.stddev();
        lixel_statistics.push_back(stat);
    }
    return lixel_statistics;
}

}  // namespace calibration
}  // namespace plenoscope
