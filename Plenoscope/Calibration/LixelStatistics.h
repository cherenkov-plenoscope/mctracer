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
#include "Plenoscope/Calibration/OnlineLixelStatistics.h"
#include <vector>

namespace Plenoscope {
    namespace Calibration {

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

    void write(const std::vector<LixelStatistic> &lixel_statistics, const std::string &path);
    std::vector<LixelStatistic> read(const std::string &path);

    }//Calibration
}//Plenoscope
#endif // __LightFieldTelescopeLixelStatistics_H_INCLUDED__ 
