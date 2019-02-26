// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_DETECTORMAP_H_
#define SCENERY_DETECTORMAP_H_

#include <stdint.h>
#include <map>
#include <vector>
#include "ResourceMap.h"
#include "merlict/sensor/Sensor.h"

namespace relleums {

struct SensorMap: public ResourceMap {
	std::vector<sensor::Sensor*> sensors;
    void add(const uint64_t key, const Frame* frame);
    ~SensorMap();
};

}  // namespace relleums

#endif  // SCENERY_DETECTORMAP_H_
