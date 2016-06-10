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
   
    struct LixelStatistic {
        uint count;
        OnlineStatistics cx;
        OnlineStatistics cy;
        OnlineStatistics x;
        OnlineStatistics y;
        OnlineStatistics time;

        LixelStatistic(): count(0) {};
    };

    class LixelStatistics {

        const Config *calib_config;
        const LightFieldSensor::Geometry *sensor_geometry;
        const double photons_emitted_per_lixel;

        vector<LixelStatistic> lixel_stats;
    public:

        LixelStatistics(
            const LightFieldSensor::Geometry *sensor_geometry, 
            const Config *config
        );
        void save(const string path);
        void fill_in_block(vector<CalibrationPhotonResult> &calib_block);
    private:

        double min_arrival_time_mean()const;
        void normalize();
    };

    }//Calibration
}//Plenoscope
#endif // __LightFieldTelescopeLixelStatistics_H_INCLUDED__ 
