// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORE_DISTANCEMETER_H_
#define MCTRACER_CORE_DISTANCEMETER_H_

#include "Core/Ray.h"
#include "Core/Frame.h"

struct DistanceMeter {
    DistanceMeter(const Ray* ray, const Frame* world);
    bool faces_an_object;
    double distance_to_closest_object;
};
#endif  // MCTRACER_CORE_DISTANCEMETER_H_
