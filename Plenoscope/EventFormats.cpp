#include "EventFormats.h"
#include <fstream> 
#include "Corsika/Tools.h"
#include "Core/TracerException.h"
#include "Tools/AsciiIo.h"

namespace Plenoscope {
//------------------------------------------------------------------------------
EventHeader::EventHeader() {
    for(uint i=0; i<raw.size(); i++)
        raw[i]=0.0;

    raw[  1-1] = Corsika::str2float("PEVT");
}
//------------------------------------------------------------------------------
void EventHeader::set_event_type(const float event_type) {
    raw[  2-1] = event_type;
}
//------------------------------------------------------------------------------
void EventHeader::set_trigger_type(const float trigger_type) {
    raw[  3-1] = trigger_type;    
}
//------------------------------------------------------------------------------
void EventHeader::set_sensor_plane_2_imaging_system(
    const HomTra3 &sensor_plane2imaging_system
) {
    raw[ 11-1] = sensor_plane2imaging_system.get_rot_x().x();
    raw[ 12-1] = sensor_plane2imaging_system.get_rot_x().y();
    raw[ 13-1] = sensor_plane2imaging_system.get_rot_x().z();

    raw[ 14-1] = sensor_plane2imaging_system.get_rot_y().x();
    raw[ 15-1] = sensor_plane2imaging_system.get_rot_y().y();
    raw[ 16-1] = sensor_plane2imaging_system.get_rot_y().z();

    raw[ 17-1] = sensor_plane2imaging_system.get_rot_z().x();
    raw[ 18-1] = sensor_plane2imaging_system.get_rot_z().y();
    raw[ 19-1] = sensor_plane2imaging_system.get_rot_z().z();

    raw[ 20-1] = sensor_plane2imaging_system.get_translation().x();
    raw[ 21-1] = sensor_plane2imaging_system.get_translation().y();
    raw[ 22-1] = sensor_plane2imaging_system.get_translation().z();  
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