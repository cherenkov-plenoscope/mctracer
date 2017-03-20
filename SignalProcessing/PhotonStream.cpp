#include "PhotonStream.h"
#include <sstream>
#include "Core/TracerException.h"
#include <math.h>
#include <fstream> 

namespace SignalProcessing {
	namespace PhotonStream {
//------------------------------------------------------------------------------
Stream::Stream() {
    slice_duration = 0.0; 
}
//------------------------------------------------------------------------------
void write(
	const vector<vector<SignalProcessing::ElectricPulse>> &pulses,
	const float slice_duration,
    const string path
) {
    uint32_t number_of_pulses_and_number_of_channels = 
        number_of_pulses(pulses) + pulses.size();

    uint32_t number_of_symbols;
    if(number_of_pulses_and_number_of_channels > 0)
        number_of_symbols = number_of_pulses_and_number_of_channels - 1;
    else
        number_of_symbols = 0;

    std::ofstream file;
    file.open(path, std::ios::binary);

    if(!file.is_open()) {
        std::stringstream info;
        info << "Can not write file '" << path << "'.\n";
        throw TracerException(info.str());
    }       

    // PhotonStream Header 
    file.write( (char*)&slice_duration, sizeof(float));
    file.write( (char*)&number_of_symbols, sizeof(uint32_t));

    // Pulses
    uint arrival_time_out_of_range_counter = 0;
    uint total_number_of_arrival_times = 0;
    for(uint channel=0; channel<pulses.size(); channel++) {
        for(uint pulse=0; pulse<pulses.at(channel).size(); pulse++) {
    		total_number_of_arrival_times++;

            int arrival_slice_32bit = round(
                pulses.at(channel).at(pulse).arrival_time/slice_duration);

            if(arrival_slice_32bit >= NEXT_READOUT_CHANNEL_MARKER ||
               arrival_slice_32bit < 0 )
                arrival_time_out_of_range_counter++;             

            const unsigned char arrival_slice_8bit = 
                (unsigned char)arrival_slice_32bit;

    		file.write( (char*)&arrival_slice_8bit, 1);
    	}
        if (channel < pulses.size()-1)
    	   file.write( (char*)&NEXT_READOUT_CHANNEL_MARKER, 1);
    }

    file.close();

    if(arrival_time_out_of_range_counter > 0.01*total_number_of_arrival_times) {
        std::stringstream info;
        info << "Over 1 percent of the arrival times are out of the range: ";
        info << "0.0ns to " << 254.0*slice_duration*1e9 << "ns.\n";
        throw TracerException(info.str());        
    }
}
//------------------------------------------------------------------------------
void write_simulation_truth(
    const vector<vector<SignalProcessing::ElectricPulse>> &pulses,
    const string path
) {
    std::ofstream file;
    file.open(path, std::ios::binary);

    if(!file.is_open()) {
        std::stringstream info;
        info << "Can not write file '" << path << "'.\n";
        throw TracerException(info.str());
    }       

    for(uint channel=0; channel<pulses.size(); channel++) {
        for(uint pulse=0; pulse<pulses.at(channel).size(); pulse++) {
            int pulse_truth_id = pulses.at(channel).at(pulse).simulation_truth_id;
            file.write( (char*)&pulse_truth_id, sizeof(int));
        }
    }

    file.close();    
}
//------------------------------------------------------------------------------
Stream read(const string path) {
    Stream stream;

    std::ifstream file;
    file.open(path, std::ios::binary);

    if(!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "PhotonStream: Unable to open file: '" << path << "'\n";
        throw TracerException(info.str());}

    file.read((char*)&stream.slice_duration, sizeof(float));
    uint32_t number_of_symbols;
    file.read((char*)&number_of_symbols, sizeof(uint32_t));

    if(number_of_symbols > 0) {
        vector<SignalProcessing::ElectricPulse> first_channel;
        stream.photon_stream.push_back(first_channel);
    }

    uint32_t channel = 0;
    for (uint32_t i=0; i<number_of_symbols; i++) {
        unsigned char symbol;
        file.read((char*)&symbol, 1);

        if (symbol == NEXT_READOUT_CHANNEL_MARKER) {
            channel++;
            if (i < number_of_symbols) {
                vector<SignalProcessing::ElectricPulse> next_channel;
                stream.photon_stream.push_back(next_channel);
            }
        }else{
            SignalProcessing::ElectricPulse pulse;
            pulse.arrival_time = symbol*stream.slice_duration;
            stream.photon_stream.at(channel).push_back(pulse);
        }
    }

    file.close();

    return stream;
}
//------------------------------------------------------------------------------
Stream read_with_simulation_truth(const string path, const string truth_path) {
    Stream stream = read(path);

    std::ifstream file;
    file.open(truth_path, std::ios::binary);
    if(!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "PhotonStream: Unable to open file: '" << truth_path << "'\n";
        throw TracerException(info.str());}

    for(uint channel=0; channel<stream.photon_stream.size(); channel++) {
        for(uint pulse=0; pulse<stream.photon_stream.at(channel).size(); pulse++) {

            int simulation_truth_id;
            file.read((char*)&simulation_truth_id, sizeof(int));
            stream.photon_stream.at(channel).at(pulse).simulation_truth_id = 
                simulation_truth_id;
        }
    }

    file.close();

    return stream;
}
//------------------------------------------------------------------------------
uint number_of_pulses(
    const vector<vector<SignalProcessing::ElectricPulse>> &pulses
) {
    uint number_pulses = 0;
    for(uint channel=0; channel<pulses.size(); channel++)
        number_pulses += pulses.at(channel).size(); 
    return number_pulses;
}
//------------------------------------------------------------------------------
	}//PhotonStream
}//SignalProcessing