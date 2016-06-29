//=================================
// include guard
#ifndef __LightFieldTelescopeLixelStatistics_H_INCLUDED__
#define __LightFieldTelescopeLixelStatistics_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/Calibration/CalibrationPhotonResult.h"
#include "Plenoscope/Calibration/Config.h"
#include "Plenoscope/LightFieldSensor/Geometry.h"
#include "Tools/OnlineStatistics.h"

namespace Plenoscope {
    namespace Calibration {
    //--------------------------------------------------------------------------
    struct LixelStatistic {
        float efficiency;
        float cx_mean, cx_std;
        float cy_mean, cy_std;
        float x_mean, x_std;
        float y_mean, y_std;
        float time_mean, time_std;
        LixelStatistic();
    };

    void write(const vector<LixelStatistic> &lixel_statistics, const string &path);
    vector<LixelStatistic> read(const string &path);

    class CanNotOpenLixelStatisticsFile : public TracerException {
        using TracerException::TracerException;
    };
    //--------------------------------------------------------------------------
    struct OnlineLixelStatistic {
        uint count;
        OnlineStatistics cx;
        OnlineStatistics cy;
        OnlineStatistics x;
        OnlineStatistics y;
        OnlineStatistics time;
        OnlineLixelStatistic();
    };
    //--------------------------------------------------------------------------
    class LixelStatisticsFiller {

        const Config *calib_config;
        const LightFieldSensor::Geometry *sensor_geometry;
        vector<OnlineLixelStatistic> lixel_stats;
    public:

        const double photons_emitted_per_lixel;
        LixelStatisticsFiller(
            const LightFieldSensor::Geometry *sensor_geometry, 
            const Config *config
        );
        vector<LixelStatistic> get_lixel_statistics()const;
        void fill_in_block(vector<CalibrationPhotonResult> &calib_block);
    private:

        double min_arrival_time_mean()const;
    };
    //--------------------------------------------------------------------------
    }//Calibration
}//Plenoscope
#endif // __LightFieldTelescopeLixelStatistics_H_INCLUDED__ 
