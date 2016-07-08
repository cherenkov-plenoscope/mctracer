#include "LixelStatistics.h"
#include "Tools/FileTools.h"

namespace Plenoscope {
    namespace Calibration {
//------------------------------------------------------------------------------
LixelStatistic::LixelStatistic():
    efficiency(0.0),
    cx_mean(0.0), cx_std(0.0), 
    cy_mean(0.0), cy_std(0.0),
    x_mean(0.0), x_std(0.0),
    y_mean(0.0), y_std(0.0),
    time_delay_mean(0.0), time_delay_std(0.0)
{}
//------------------------------------------------------------------------------
void write(const vector<LixelStatistic> &lixel_statistics, const string &path) {

    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if(!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "LixelStatistics: Unable to write file: '" << path << "'\n";
        throw CanNotOpenLixelStatisticsFile(info.str());      
    }

    for(const LixelStatistic &lixel: lixel_statistics)
        file.write((char*)&lixel, sizeof(LixelStatistic));
    file.close();
}
//------------------------------------------------------------------------------
vector<LixelStatistic> read(const string &path) {

    std::ifstream file;
    file.open(path, std::ios::binary);
    if(!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "LixelStatistics: Unable to read file: '" << path << "'\n";
        throw CanNotOpenLixelStatisticsFile(info.str());      
    }

    vector<LixelStatistic> lixel_statistics;
    while(true) {
        LixelStatistic lixel;
        file.read((char*)&lixel, sizeof(LixelStatistic));
        if(!file.eof())
            lixel_statistics.push_back(lixel);
        else
            break;
    }
    file.close();
    return lixel_statistics;
}
//------------------------------------------------------------------------------
OnlineLixelStatistic::OnlineLixelStatistic(): count(0) {}
//------------------------------------------------------------------------------
LixelStatisticsFiller::LixelStatisticsFiller(
    const LightFieldSensor::Geometry *geometry,  
    const Config *_calib_config
):
    calib_config(_calib_config),
    sensor_geometry(geometry),
    photons_emitted_per_lixel(
        double(_calib_config->number_of_blocks) * double(_calib_config->photons_per_block) /
        double(geometry->number_of_lixel())
    )
 {
    lixel_stats.resize(sensor_geometry->number_of_lixel());
}
//------------------------------------------------------------------------------
void LixelStatisticsFiller::fill_in_block(vector<CalibrationPhotonResult> &calib_block) {

    for(CalibrationPhotonResult result: calib_block) {
        if(result.reached_sensor == true) {
            lixel_stats[result.lixel_id].count++;
            lixel_stats[result.lixel_id].cx.add(result.x_tilt_vs_optical_axis);
            lixel_stats[result.lixel_id].cy.add(result.y_tilt_vs_optical_axis);
            lixel_stats[result.lixel_id].x.add(result.x_pos_on_principal_aperture);
            lixel_stats[result.lixel_id].y.add(result.y_pos_on_principal_aperture);
            lixel_stats[result.lixel_id].timed_delay.add(result.relative_time_of_flight);
        }
    }
}
//------------------------------------------------------------------------------
double LixelStatisticsFiller::min_arrival_time_mean()const {

    double minimal_time_mean = 0.0;
    if(lixel_stats.size() > 0)
        minimal_time_mean = lixel_stats.at(0).timed_delay.mean();

    for(OnlineLixelStatistic lixel: lixel_stats) {
        double this_pix_time = lixel.timed_delay.mean();
        if(this_pix_time < minimal_time_mean)
            minimal_time_mean = this_pix_time;
    }
    return minimal_time_mean;
}
//------------------------------------------------------------------------------
vector<LixelStatistic> LixelStatisticsFiller::get_lixel_statistics()const {
    const double minimal_arrival_time = min_arrival_time_mean();

    vector<LixelStatistic> lixel_statistics;
    lixel_statistics.reserve(lixel_stats.size());

    for(const OnlineLixelStatistic &lixel: lixel_stats) {
        LixelStatistic stat;
        stat.efficiency = lixel.count/photons_emitted_per_lixel;
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
//------------------------------------------------------------------------------
    }//Calibration
}//Plenoscope