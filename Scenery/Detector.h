//=================================
// include guard
#ifndef __DETECTOR_H_INCLUDED__
#define __DETECTOR_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies


#include "Core/Frame.h"

struct Detector {

    Frame* frame;
    PhotonSensors::Sensors* sensors;
};
#endif // __DETECTOR_H_INCLUDED__