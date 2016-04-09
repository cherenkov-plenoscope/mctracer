//=================================
// include guard
#ifndef __LightFieldTelescopePlenoscope_H_INCLUDED__
#define __LightFieldTelescopePlenoscope_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldTelescope/Config.h"
#include "LightFieldTelescope/Geometry.h"
#include "LightFieldTelescope/Factory.h"
using std::vector;
using std::string;
using std::stringstream;

namespace LightFieldTelescope {
    
    struct Plenoscope {
        Config cfg;
        Geometry geometry;
        Factory factory;
        PhotonSensors::Sensors* sensors;

        Plenoscope(Config cfg);
    };
}
#endif // __LightFieldTelescopePlenoscope_H_INCLUDED__ 
