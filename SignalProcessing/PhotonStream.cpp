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
    const uint32_t number_channels = pulses.size();

    const uint32_t number_symbols = number_of_symbols_to_represent_pulses(
        pulses);

    const uint32_t number_time_slices = 100u;
    assert_number_time_slices_below_8bit_max(number_time_slices);

    std::ofstream file;
    file.open(path, std::ios::binary);

    if(!file.is_open()) {
        std::stringstream info;
        info << "Can not write file '" << path << "'.\n";
        throw TracerException(info.str());
    }       

    // PhotonStream Header 16 byte
    // -------------------
    file.write( (char*)&slice_duration, sizeof(float));
    file.write( (char*)&number_channels, sizeof(uint32_t));
    file.write( (char*)&number_time_slices, sizeof(uint32_t));
    file.write( (char*)&number_symbols, sizeof(uint32_t));

    // Pulses
    // ------
    for(uint channel=0; channel<number_channels; channel++) {
        for(uint pulse=0; pulse<pulses.at(channel).size(); pulse++) {

            int arrival_slice_32bit = round(
                pulses.at(channel).at(pulse).arrival_time/slice_duration);           

            const unsigned char arrival_slice_8bit = 
                (unsigned char)arrival_slice_32bit;

    		file.write( (char*)&arrival_slice_8bit, 1);
    	}
        if (channel < number_channels-1)
    	   file.write( (char*)&NEXT_READOUT_CHANNEL_MARKER, 1);
    }

    file.close();
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

    uint32_t number_channels;
    file.read((char*)&number_channels, sizeof(uint32_t));
  
    uint32_t number_time_slices;
    file.read((char*)&number_time_slices, sizeof(uint32_t));
    assert_number_time_slices_below_8bit_max(number_time_slices);

    uint32_t number_symbols;
    file.read((char*)&number_symbols, sizeof(uint32_t));

    if(number_symbols > 0) {
        vector<SignalProcessing::ElectricPulse> first_channel;
        stream.photon_stream.push_back(first_channel);
    }

    uint32_t channel = 0;
    for (uint32_t i=0; i<number_symbols; i++) {
        unsigned char symbol;
        file.read((char*)&symbol, 1);

        if (symbol == NEXT_READOUT_CHANNEL_MARKER) {
            channel++;
            if (i < number_symbols) {
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
uint number_of_symbols_to_represent_pulses(
    const vector<vector<SignalProcessing::ElectricPulse>> &pulses
) {
    uint32_t number_of_pulses_and_number_of_channels = 
        number_of_pulses(pulses) + pulses.size();

    uint32_t number_symbols;
    if(number_of_pulses_and_number_of_channels > 0)
        number_symbols = number_of_pulses_and_number_of_channels - 1;
    else
        number_symbols = 0;
    return number_symbols;
}
//------------------------------------------------------------------------------
void assert_number_time_slices_below_8bit_max(
    const uint32_t number_time_slices) {

    if(number_time_slices > NEXT_READOUT_CHANNEL_MARKER) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "Expected number_time_slices <= NEXT_READOUT_CHANNEL_MARKER ";
        info << "but actual number_time_slices = ";
        info << number_time_slices << ", ";
        info << "and NEXT_READOUT_CHANNEL_MARKER = ";
        info << NEXT_READOUT_CHANNEL_MARKER << ".\n";
        throw TracerException(info.str());
    }
}
//------------------------------------------------------------------------------
	}//PhotonStream
}//SignalProcessing