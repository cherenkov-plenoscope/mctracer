// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_DISTANCEMETER_H_
#define CORE_DISTANCEMETER_H_

#include "merlict/Ray.h"
#include "merlict/Frame.h"

namespace relleums {

struct DistanceMeter {
    DistanceMeter(const Ray* ray, const Frame* world);
    bool faces_an_object;
    double distance_to_closest_object;
};

}  // namespace relleums

#endif  // CORE_DISTANCEMETER_H_
