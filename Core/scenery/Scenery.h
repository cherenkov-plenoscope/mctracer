// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_SCENERY_H_
#define SCENERY_SCENERY_H_

#include "ColorMap.h"
#include "FunctionMap.h"
#include "SensorMap.h"
#include "merlict/Frame.h"

namespace relleums {

struct Scenery {
    Frame root;
    ColorMap colors;
    FunctionMap functions;
    SensorMap sensors;

    std::string current_working_directory;

    Scenery();
    static Scenery void_scenery;
};

}  // namespace relleums

#endif  // SCENERY_SCENERY_H_
