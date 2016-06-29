//=================================
// include guard
#ifndef __LightFieldTelescopeCalibrationConfig_H_INCLUDED__
#define __LightFieldTelescopeCalibrationConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <stdint.h>
#include <string>
using std::string;

namespace Plenoscope {
    namespace Calibration {

    struct Config {
        uint32_t number_of_blocks;
        uint32_t photons_per_block;
        Config();
    };

    }//Calibration
}//Plenoscope
#endif // __LightFieldTelescopeCalibrationConfig_H_INCLUDED__ 