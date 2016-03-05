//=================================
// include guard
#ifndef __DistanceMeter_H_INCLUDED__
#define __DistanceMeter_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Ray.h"
#include <limits.h>

class DistanceMeter {
	bool does_DistanceMeter_face_an_object = false;
	double distance_to_closest_object = std::numeric_limits<double>::max();
public:
	DistanceMeter(const Ray* ray, const Frame* world);
	bool does_face_an_object()const;
	double get_distance_to_closest_object()const;
};
#endif // __DistanceMeter_H_INCLUDED__ 