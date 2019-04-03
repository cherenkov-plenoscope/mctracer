// Copyright 2016 Sebastian A. Mueller
#ifndef PLENOSCOPE_NIGHTSKYBACKGROUND_NIGHTSKYBACKGROUND_H_
#define PLENOSCOPE_NIGHTSKYBACKGROUND_NIGHTSKYBACKGROUND_H_

#include "merlict_portal_plenoscope/night_sky_background/Injector.h"
#include "merlict_portal_plenoscope/night_sky_background/Light.h"

namespace plenoscope {
namespace night_sky_background {

const double APERTURE_RADIUS_OVERHEAD = 1.05;
const double FOV_RADIUS_OVERHEAD = 1.20;
// For the target position and orientation of the
// light field sensor relative to the imaging reflector,
// a 1.05 overhead would be sufficient, but to explore
// also stronger misalignments, we spear the incoming
// photon directions more out.

}  // namespace night_sky_background
}  // namespace plenoscope

#endif  // PLENOSCOPE_NIGHTSKYBACKGROUND_NIGHTSKYBACKGROUND_H_
