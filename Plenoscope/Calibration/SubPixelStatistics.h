//=================================
// include guard
#ifndef __LightFieldTelescopeSubPixelStatistics_H_INCLUDED__
#define __LightFieldTelescopeSubPixelStatistics_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/Calibration/CalibrationPhotonResult.h"
#include "Plenoscope/Calibration/Config.h"
#include "Plenoscope/LightFieldSensor/Geometry.h"

namespace Plenoscope {
    namespace Calibration {
   
    struct SubPixelStatistic {
        double geometric_efficiency;
        double mean_cx;
        double mean_cy;
        double mean_x;
        double mean_y;
        double mean_time;

        SubPixelStatistic() {
            geometric_efficiency = 0.0;
            mean_cx = 0.0;
            mean_cy = 0.0;
            mean_x = 0.0;
            mean_y = 0.0;
            mean_time = 0.0;            
        }
    };

    class SubPixelStatistics {

        const Config *calib_config;
        const LightFieldSensor::Geometry *sensor_geometry;
        const double photons_per_sub_pixel;

        std::vector<SubPixelStatistic> subpix_stats;
    public:

        SubPixelStatistics(
            const LightFieldSensor::Geometry *sensor_geometry, 
            const Config *config
        );
        void save(const std::string path);
        void fill_in_block(std::vector<CalibrationPhotonResult> &calib_block);
    private:
        void normalize();
    };

    }//Calibration
}//Plenoscope
#endif // __LightFieldTelescopeSubPixelStatistics_H_INCLUDED__ 
