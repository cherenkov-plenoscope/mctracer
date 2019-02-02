// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_DETECTORMAP_H_
#define SCENERY_DETECTORMAP_H_

#include <stdint.h>
#include <map>
#include <vector>
#include "ResourceMap.h"
#include "PhotonSensor/Sensor.h"

struct SensorMap: public ResourceMap {
	std::vector<PhotonSensor::Sensor*> sensors;
    void add(const uint64_t key, const Frame* frame);
    ~SensorMap();
};

#endif  // SCENERY_DETECTORMAP_H_
