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
using std::vector;
using std::string;
using std::stringstream;

namespace Plenoscope {
    
    void save_event_to_file_epoch_2016May27(
        const vector<SimpleTdcQdc::TimeAndCount> &tacs,
        const string path
    );

    void save_event_to_file_epoch_2016Mar10(
        const vector<SimpleTdcQdc::TimeAndCount> &tacs,
        const string path,
        const EventIo::Event &event,
        const EventIo::Run &corsika_run
    );
}
#endif // __LightFieldTelescopeEventFormats_H_INCLUDED__ 
