//=================================
// include guard
#ifndef __PlenoscopeCalibrationLixelStatisticsFiller_H_INCLUDED__
#define __PlenoscopeCalibrationLixelStatisticsFiller_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/Calibration/CalibrationPhotonResult.h"
#include "Plenoscope/Calibration/Config.h"
#include "Plenoscope/LightFieldSensor/Geometry.h"
#include "Plenoscope/Calibration/OnlineLixelStatistics.h"
#include "Plenoscope/Calibration/LixelStatistics.h"

namespace Plenoscope {
    namespace Calibration {

    class LixelStatisticsFiller {

        const Config *calib_config;
        const LightFieldSensor::Geometry *sensor_geometry;
        vector<OnlineLixelStatistics> lixel_stats;
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

    }//Calibration
}//Plenoscope
#endif // __PlenoscopeCalibrationLixelStatisticsFiller_H_INCLUDED__ 
