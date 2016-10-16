//=================================
// include guard
#ifndef __LightFieldTelescopeEventFormats_H_INCLUDED__
#define __LightFieldTelescopeEventFormats_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <vector>
#include <string>
#include <sstream>
#include "SignalProcessing/SimpleTDCQDC.h"
#include "Corsika/EventIo/EventIo.h"
#include "LightFieldSensor/Config.h"
using std::vector;
using std::string;
using std::stringstream;

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
        void set_event_type(const float event_type);
        void set_trigger_type(const float trigger_type);
        void set_plenoscope_geometry(const LightFieldSensor::Config &cfg);
    };

    void save_event_to_file_epoch_2016May27(
        const vector<SignalProcessing::SimpleTdcQdc::TimeAndCount> &tacs,
        const string path
    );

    void save_event_to_file_epoch_2016Mar10(
        const vector<SignalProcessing::SimpleTdcQdc::TimeAndCount> &tacs,
        const string path,
        const EventIo::Event &event,
        const EventIo::Run &corsika_run
    );
}
#endif // __LightFieldTelescopeEventFormats_H_INCLUDED__ 
