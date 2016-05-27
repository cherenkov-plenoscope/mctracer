#include "EventFormats.h"
#include <fstream> 
#include "Core/TracerException.h"
#include "Tools/AsciiIo.h"

namespace Plenoscope {
//------------------------------------------------------------------------------
void save_event_to_file_epoch_2016May27(
    const vector<SimpleTdcQdc::TimeAndCount> &tacs,
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
    for(SimpleTdcQdc::TimeAndCount tac: tacs) {
        const float ct = float(tac.count);
        float v[float_count] = {tac.time, ct};
        file.write( (char*)&v, float_count*sizeof(float));  
    }

    file.close();
}
//------------------------------------------------------------------------------
void save_event_to_file_epoch_2016Mar10(
    const vector<SimpleTdcQdc::TimeAndCount> &tacs,
    const string path,
    const EventIo::Event &event,
    const EventIo::EventIoFile &corsika_run
) {
    vector<vector<double>> dtacs;
    for(SimpleTdcQdc::TimeAndCount tac: tacs) {
        const double tm = tac.time;
        const double ct = double(tac.count);
        vector<double> dtac = {tm, ct};
        dtacs.push_back(dtac);
    }

    stringstream evt_header;
    evt_header << corsika_run.run_header.mmcs_runheader.get_print();
    evt_header << event.header.mmcs_event_header.get_print();
    evt_header << "\n";
    evt_header << "arrival_time[s]\tnumber_photons[1]\n";

    AsciiIo::write_table_to_file_with_header(dtacs, path, evt_header.str());
}
//------------------------------------------------------------------------------
}//Plenoscope