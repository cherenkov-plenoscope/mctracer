#include "EventFormats.h"
#include <fstream> 
#include "Corsika/Tools.h"
#include "Core/TracerException.h"
#include "Tools/AsciiIo.h"
#include "SignalProcessing/PhotonStream.h"

namespace Plenoscope {
//------------------------------------------------------------------------------
void save_event_to_file_epoch_2017Mar19(
    const vector<vector<SignalProcessing::ElectricPulse>> &electric_pipelines,
    const double slice_duration,
    const string path
) {
    SignalProcessing::PhotonStream::write(
        electric_pipelines,
        slice_duration,
        path
    );
}
//------------------------------------------------------------------------------
void save_event_to_file_epoch_2016May27(
    const vector<SignalProcessing::SimpleTdcQdc::TimeAndCount> &tacs,
    const string path
) {
    std::ofstream file;
    file.open(path, std::ios::binary);

    if(!file.is_open()) {
        std::stringstream info;
        info << "Can not write file '" << path << "'.\n";
        throw TracerException(info.str());       
    }   

    const uint float_count = 2;
    for(SignalProcessing::SimpleTdcQdc::TimeAndCount tac: tacs) {
        const float ct = float(tac.count);
        const float tm = float(tac.time);
        float v[float_count] = {tm, ct};
        file.write( (char*)&v, float_count*sizeof(float));  
    }

    file.close();
}
//------------------------------------------------------------------------------
void save_event_to_file_epoch_2016Mar10(
    const vector<SignalProcessing::SimpleTdcQdc::TimeAndCount> &tacs,
    const string path,
    const EventIo::Event &event,
    const EventIo::Run &corsika_run
) {
    vector<vector<double>> dtacs;
    for(SignalProcessing::SimpleTdcQdc::TimeAndCount tac: tacs) {
        const double tm = tac.time;
        const double ct = double(tac.count);
        vector<double> dtac = {tm, ct};
        dtacs.push_back(dtac);
    }

    stringstream evt_header;
    evt_header << Corsika::RunHeader::get_print(corsika_run.header.raw);
    evt_header << Corsika::EventHeader::get_print(event.header.raw);
    evt_header << "\n";
    evt_header << "arrival_time[s]\tnumber_photons[1]\n";

    AsciiIo::write_table_to_file_with_header(dtacs, path, evt_header.str());
}
//------------------------------------------------------------------------------
}//Plenoscope