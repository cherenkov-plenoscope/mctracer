//=================================
// include guard
#ifndef __PlenoscopeEventHeader_H_INCLUDED__
#define __PlenoscopeEventHeader_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldSensor/Config.h"
#include <stdint.h>

namespace Plenoscope {
    
    namespace EventTypes {
        static const float OBSERVATION = 0.0; 
        static const float SIMULATION = 1.0;
    }//EventTypes

    namespace TriggerType {
        static const float SELF_TRIGGER = 0.0; 
        static const float EXTERNAL_RANDOM_TRIGGER = 1.0;
        static const float EXTERNAL_TRIGGER_BASED_ON_AIR_SHOWER_SIMULATION_TRUTH = 2.0;
    }//TriggerType

    struct EventHeader {
        array<float, 273> raw;
        EventHeader();
        void set_random_number_seed_of_run(const uint32_t seed);
        void set_event_type(const float event_type);
        void set_trigger_type(const float trigger_type);
        void set_plenoscope_geometry(const LightFieldSensor::Config &cfg);
    };
}
#endif // __PlenoscopeEventHeader_H_INCLUDED__ 
