// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_SCENERY_H_
#define SCENERY_SCENERY_H_

#include "ColorMap.h"
#include "FunctionMap.h"
#include "SensorMap.h"
#include "Core/Frame.h"

struct Scenery {
    Frame root;
    ColorMap colors;
    FunctionMap functions;
    SensorMap sensors;

    Scenery();
    static Scenery void_scenery;
};

#endif  // SCENERY_SCENERY_H_
