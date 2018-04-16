// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_PLENOSCOPEINSCENERY_H_
#define PLENOSCOPE_PLENOSCOPEINSCENERY_H_

#include "Plenoscope/LightFieldSensor/LightFieldSensor.h"

namespace Plenoscope {

struct PlenoscopeInScenery {
PlenoscopeInScenery(const LightFieldSensor::Config config):
        light_field_sensor_geometry(config)
    {};
    Frame* frame;
    PhotonSensor::Sensors* light_field_channels;
    LightFieldSensor::Geometry light_field_sensor_geometry;
};

}  // namespace Plenoscope

#endif  // PLENOSCOPE_PLENOSCOPEINSCENERY_H_
