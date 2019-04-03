// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_PLENOSCOPEINSCENERY_H_
#define PLENOSCOPE_PLENOSCOPEINSCENERY_H_

#include "merlict_portal_plenoscope/light_field_sensor/LightFieldSensor.h"
#include "merlict/merlict.h"

namespace plenoscope {

struct PlenoscopeInScenery {

	PlenoscopeInScenery(const light_field_sensor::Config config):
        light_field_sensor_geometry(config)
    {};

    merlict::Frame* frame;
    merlict::sensor::Sensors* light_field_channels;
    light_field_sensor::Geometry light_field_sensor_geometry;
};

}  // namespace plenoscope

#endif  // PLENOSCOPE_PLENOSCOPEINSCENERY_H_
