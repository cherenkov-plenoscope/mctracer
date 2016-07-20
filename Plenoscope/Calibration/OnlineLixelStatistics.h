//=================================
// include guard
#ifndef __PlenoscopeCalibrationOnloneLixelStatistics_H_INCLUDED__
#define __PlenoscopeCalibrationOnloneLixelStatistics_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Tools/OnlineStatistics.h"

namespace Plenoscope {
    namespace Calibration {

    struct OnlineLixelStatistics {
        unsigned int count;
        OnlineStatistics cx;
        OnlineStatistics cy;
        OnlineStatistics x;
        OnlineStatistics y;
        OnlineStatistics timed_delay;
        OnlineLixelStatistics();
    };

    }//Calibration
}//Plenoscope
#endif // __PlenoscopeCalibrationOnloneLixelStatistics_H_INCLUDED__ 
