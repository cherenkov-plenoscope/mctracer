// Copyright 2014 Sebastian A. Mueller
#include "merlict/sensor/Sensors.h"
#include <algorithm>
#include <sstream>
#include "merlict/sensor/FindSensorByFrame.h"
using std::vector;

namespace merlict {
namespace sensor {

vector<Sensor*> sort_by_frame(const vector<Sensor*>* by_occurence) {
    vector<Sensor*> by_frame(by_occurence->begin(), by_occurence->end());
    std::sort(
        by_frame.begin(),
        by_frame.end(),
        [&](const Sensor* a, const Sensor* b) {return a->frame < b->frame;});
    return by_frame;
}

void assign_photon_to_sensors(
    const Photon* photon,
    vector<Sensor*>* sensors_by_frame
) {
    FindSensorByFrame finder(
        photon->get_final_intersection().get_object(),
        sensors_by_frame);
    if (finder.is_absorbed_by_known_sensor)
        finder.final_sensor->assign_photon(photon);
}

void assign_photons_to_sensors(
    const vector<Photon>* photons,
    vector<Sensor*>* sensors_by_frame
) {
    for (const Photon &photon : *photons)
        assign_photon_to_sensors(&photon, sensors_by_frame);
}

Sensors::Sensors() {}

Sensors::Sensors(const vector<Sensor*> &sensors) {
    init(sensors);
}

void Sensors::init(const vector<Sensor*> &sensors) {
    by_occurence.clear();
    by_frame.clear();
    by_occurence = sensors;
    by_frame = sort_by_frame(&by_occurence);
    assert_no_two_sensors_have_same_frame();
}

unsigned int Sensors::size()const {
    return by_occurence.size();
}

Sensor* Sensors::at(const unsigned int pos) {
    return by_occurence.at(pos);
}

Sensor* Sensors::at_frame(const Frame* frame) {
    FindSensorByFrame finder(frame, &by_frame);
    if (!finder.is_absorbed_by_known_sensor) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "There is no sensor for a frame called '";
        info << frame->get_path_in_tree_of_frames();
        info << "', '" << frame->get_name();
        info << "' in the list of " << by_frame.size() << " sensors.";
        throw NoSuchFrame(info.str());
    }
    return finder.final_sensor;
}

void Sensors::assign_photon(const Photon* photon) {
    assign_photon_to_sensors(photon, &by_frame);
}

void Sensors::assign_photons(const vector<Photon>* photons) {
    assign_photons_to_sensors(photons, &by_frame);
}

void Sensors::clear_history() {
    for (Sensor* sensor : by_occurence)
        sensor->photon_arrival_history.clear();
}

void Sensors::assert_no_two_sensors_have_same_frame()const {
    for (unsigned int i = 1; i < by_frame.size(); i++) {
        if (by_frame.at(i)->frame == by_frame.at(i-1)->frame) {
            std::stringstream info;
            info << __FILE__ << ", " << __LINE__ << "\n";
            info << "The sensors at " << i << " and " << i-1 << " share the ";
            info << "same frame '";
            info << by_frame.at(i)->frame->get_path_in_tree_of_frames();
            info << "'\n";
            throw DuplicateFrame(info.str());
        }
    }
}

}  // namespace sensor
}  // namespace relleums
