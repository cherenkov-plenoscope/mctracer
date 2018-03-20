// Copyright 2016 Sebastian A. Mueller
#ifndef PLENOSCOPE_NIGHTSKYBACKGROUND_NIGHTSKYBACKGROUND_H_
#define PLENOSCOPE_NIGHTSKYBACKGROUND_NIGHTSKYBACKGROUND_H_

#include "Plenoscope/NightSkyBackground/Injector.h"
#include "Plenoscope/NightSkyBackground/Light.h"

namespace Plenoscope {
namespace NightSkyBackground {

const double APERTURE_RADIUS_OVERHEAD = 1.05;
const double FOV_RADIUS_OVERHEAD = 1.20;
// For the target position and orientation of the
// light field sensor relative to the imaging reflector,
// a 1.05 overhead would be sufficient, but to explore
// also stronger misalignments, we spear the incoming
// photon directions more out.

}  // namespace NightSkyBackground
}  // namespace Plenoscope

#endif  // PLENOSCOPE_NIGHTSKYBACKGROUND_NIGHTSKYBACKGROUND_H_
