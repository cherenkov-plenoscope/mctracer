//=================================
// include guard
#ifndef __ZenithDistanceAzimuthMount_H_INCLUDED__
#define __ZenithDistanceAzimuthMount_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Frame.h"

namespace ZdAzMount {

	Rot3 get_rot_Az_Zd(const double az, const double zd);
	double get_zd_of_frame(const Frame* frame);
	double get_az_of_frame(const Frame* frame);
} // AltitudeAzimuthMount

#endif // __ZenithDistanceAzimuthMount_H_INCLUDED__