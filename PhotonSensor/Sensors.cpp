// Copyright 2014 Sebastian A. Mueller
#include "PhotonSensor/Sensors.h"
#include "Finder.h"
#include <algorithm>
#include <sstream>
using std::vector;

namespace PhotonSensor {

Sensors::Sensors() {}

Sensors::Sensors(vector<PhotonSensor::Sensor*> &sensors) {
    init(sensors);
}

void Sensors::init(vector<PhotonSensor::Sensor*> &sensors) {
    by_occurence.clear();
    by_frame.clear();
    occurence2frame_indices.clear();
    by_occurence = std::move(sensors);
    for (unsigned int i = 0; i < by_occurence.size(); i++)
        by_occurence[i]->id = i;
    init_indices_occurence2frame();
    init_sensors_by_frames();
    sensors.clear();
}

unsigned int Sensors::size()const {
    return by_occurence.size();
}

Sensor* Sensors::at(const unsigned int pos) {
    return by_occurence.at(pos);
}

Sensor* Sensors::at_frame(const Frame* frame) {
    FindSensorByFrame finder(frame, &by_frame);
    if (!finder.frame_is_in_sensors()) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "There is no sensor for a frame called '";
        info << frame->get_path_in_tree_of_frames();
        info << "', '" << frame->get_name();
        info << "' in the list of " << by_frame.size() << " sensors.";
        throw NoSuchFrame(info.str());
    }
    return finder.get_sensor();
}

void Sensors::assign_photon(const Photon* photon) {
    FindSensorByFrame finder(
        photon->get_final_intersection().get_object(),
        &by_frame);
    if (finder.frame_is_in_sensors())
        finder.get_sensor()->assign_photon_to_this_sensor(photon);
}

void Sensors::assign_photons(const vector<Photon> *photons) {
    for (const Photon &photon : *photons)
        assign_photon(&photon);
}

void Sensors::clear_history() {
    for (Sensor* sensor : by_occurence)
        sensor->clear_history();
}

void Sensors::assert_no_two_sensors_have_same_frame()const {
    // look for duplicate sensor frames in the sensor list sorted by frames
    for (unsigned int i = 1; i < by_frame.size(); i++) {
        if (by_frame.at(i)->get_frame() == by_frame.at(i-1)->get_frame()) {
            std::stringstream info;
            info << __FILE__ << ", " << __LINE__ << "\n";
            info << "The sensors at " << i << " and " << i-1 << " share the ";
            info << "same frame '";
            info << by_frame.at(i)->get_frame()->get_path_in_tree_of_frames();
            info << "'\n";
            throw DuplicateFrame(info.str());
        }
    }
}

void Sensors::init_indices_occurence2frame() {
    occurence2frame_indices.reserve(by_occurence.size());
    for (unsigned int i = 0; i < by_occurence.size(); i++)
        occurence2frame_indices.push_back(i);
    std::sort(
        occurence2frame_indices.begin(),
        occurence2frame_indices.end(),
        [&](const int& a, const int& b) {
            return by_occurence[a]->get_frame() < by_occurence[b]->get_frame();
        });
}

void Sensors::init_sensors_by_frames() {
    by_frame.reserve(by_occurence.size());
    for (unsigned int i = 0; i < by_occurence.size(); i++)
        by_frame.push_back(by_occurence[occurence2frame_indices[i]]);
    assert_no_two_sensors_have_same_frame();
}

}  // namespace PhotonSensor
