//=================================
// include guard
#ifndef __LightFieldTelescopePlenoscope_H_INCLUDED__
#define __LightFieldTelescopePlenoscope_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/Config.h"
#include "Plenoscope/Geometry.h"
#include "Plenoscope/Factory.h"
using std::vector;
using std::string;
using std::stringstream;

namespace Plenoscope {
    
    struct Plenoscope {
        Config cfg;
        Geometry geometry;
        Factory factory;
        PhotonSensors::Sensors* sensors;

        Plenoscope(Config cfg);
    };
}
#endif // __LightFieldTelescopePlenoscope_H_INCLUDED__ 
