// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/SensorMap.h"
#include <sstream>


namespace merlict {

void SensorMap::add(const uint64_t key, const Frame* frame) {
    if (key == sensors.size()) {
        sensor::Sensor* sens = new sensor::Sensor(key, frame);
        sensors.push_back(sens);
    } else {
        std::stringstream info;
        info << "SensorMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "Expected sensors to occur in order starting with 0. ";
        info << "Expected next sensor-id to be '" << sensors.size();
        info << "', but actually ";
        info << "it is '" << key << "'.\n";
        throw std::invalid_argument(info.str());
    }
}

SensorMap::~SensorMap() {
    for (sensor::Sensor* sens : sensors)
        delete sens;
}

}  // namespace merlict
