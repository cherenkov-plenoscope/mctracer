// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_DETECTORMAP_H_
#define SCENERY_DETECTORMAP_H_

#include <string>
#include <map>
#include <vector>
#include "ResourceMap.h"
#include "PhotonSensor/Sensor.h"

struct DetectorMap: public ResourceMap {
    std::map<std::string, std::vector<PhotonSensor::Sensor*>> detectors;
    bool has(const std::string key)const;
    std::vector<PhotonSensor::Sensor*>* get(const std::string key);
    void add(
        const std::string key,
        const std::vector<PhotonSensor::Sensor*> detector);
    void assert_has(const std::string key)const;
    void assert_not_in_use_yet(const std::string key);
};

#endif  // SCENERY_DETECTORMAP_H_
