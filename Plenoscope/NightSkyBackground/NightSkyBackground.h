//=================================
// include guard
#ifndef __PlenopscopeNightSkyBackgroundNightSkyBackground_H_INCLUDED__
#define __PlenopscopeNightSkyBackgroundNightSkyBackground_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
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

	}// NightSkyBackground
}// Plenoscope

#endif // __PlenopscopeNightSkyBackgroundNightSkyBackground_H_INCLUDED__ 
