// Copyright 2014 Sebastian A. Mueller
#include "Scenery/DetectorMap.h"
#include <utility>
#include <sstream>
using std::stringstream;
using std::string;
using std::vector;
using std::map;

bool DetectorMap::has(const string key)const {
    return detectors.find(key) != detectors.end();
}

vector<PhotonSensor::Sensor*>* DetectorMap::get(const string key) {
    assert_has(key);
    return &detectors.find(key)->second;
}

void DetectorMap::add(
    const string key,
    const vector<PhotonSensor::Sensor*> detector
) {
    assert_not_in_use_yet(key);
    detectors.insert(
        std::pair<string, vector<PhotonSensor::Sensor*>>(
            key, detector));
}

void DetectorMap::assert_has(const string key)const {
    if (!has(key)) {
        stringstream info;
        info << "DetectorMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "There is no color called '" << key << "'. \n";
        throw NoSuchKey(info.str());
    }
}

void DetectorMap::assert_not_in_use_yet(const string key) {
    if (has(key)) {
        stringstream info;
        info << "DetectorMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "The key '" << key << "' is already in use.\n";
        throw KeyAlreadyInUse(info.str());
    }
}
