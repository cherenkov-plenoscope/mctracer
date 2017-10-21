//=================================
// include guard
#ifndef __DistanceMeter_H_INCLUDED__
#define __DistanceMeter_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Ray.h"
#include "Core/Frame.h"

struct DistanceMeter {
    DistanceMeter(const Ray* ray, const Frame* world);
    bool faces_an_object;
    double distance_to_closest_object;
};
#endif // __DistanceMeter_H_INCLUDED__ 