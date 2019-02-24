// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_PLENOSCOPEINSCENERY_H_
#define PLENOSCOPE_PLENOSCOPEINSCENERY_H_

#include "plenoscope/light_field_sensor/LightFieldSensor.h"
#include "Core/mctracer.h"

namespace plenoscope {

struct PlenoscopeInScenery {

	PlenoscopeInScenery(const light_field_sensor::Config config):
        light_field_sensor_geometry(config)
    {};

    relleums::Frame* frame;
    relleums::sensor::Sensors* light_field_channels;
    light_field_sensor::Geometry light_field_sensor_geometry;
};

}  // namespace plenoscope

#endif  // PLENOSCOPE_PLENOSCOPEINSCENERY_H_
