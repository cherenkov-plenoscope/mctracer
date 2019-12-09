// Copyright 2014 Sebastian A. Mueller
#include "merlict/sensor/FindSensorByFrame.h"
#include <algorithm>


namespace merlict {
namespace sensor {

FindSensorByFrame::FindSensorByFrame(
    std::shared_ptr<const Frame> final_frame,
    const std::vector<Sensor*>* sensors_by_frame
) {
    std::vector<sensor::Sensor*>::const_iterator it = std::upper_bound(
        sensors_by_frame->begin(),
        sensors_by_frame->end(),
        final_frame,
        Sensor::FrameSensorByFramePointerCompare());

    is_absorbed_by_known_sensor = true;

    if (it == sensors_by_frame->begin()) {
        // final frame pointer is smaller than all known sensor frame pointers
        is_absorbed_by_known_sensor = false;
        return;
    }

    if (it == sensors_by_frame->end() && (*(it-1))->frame != final_frame) {
        // final frame pointer is latger than all known sensor frame pointers
        is_absorbed_by_known_sensor = false;
        return;
    }

    if ((*(it-1))->frame != final_frame) {
        is_absorbed_by_known_sensor = false;
        return;
    }

    final_sensor = (*(it-1));
}

}  // namespace sensor
}  // namespace merlict
