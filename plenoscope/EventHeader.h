// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_EVENTHEADER_H_
#define PLENOSCOPE_EVENTHEADER_H_

#include <array>
#include "LightFieldSensor/Config.h"

namespace plenoscope {

namespace EventTypes {
static const float OBSERVATION = 0.0;
static const float SIMULATION = 1.0;
}  // namespace EventTypes

namespace TriggerType {
static const float SELF_TRIGGER = 0.0;
static const float EXTERNAL_RANDOM_TRIGGER = 1.0;
static const float EXTERNAL_TRIGGER_BASED_ON_AIR_SHOWER_SIMULATION_TRUTH = 2.0;
}  // namespace TriggerType

struct EventHeader {
    std::array<float, 273> raw;
    EventHeader();
    void set_event_type(const float event_type);
    void set_trigger_type(const float trigger_type);
    void set_plenoscope_geometry(const LightFieldSensor::Config &cfg);
};

}  // namespace plenoscope

#endif  // PLENOSCOPE_EVENTHEADER_H_
