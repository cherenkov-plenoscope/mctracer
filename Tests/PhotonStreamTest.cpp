#include "gtest/gtest.h"
#include "SignalProcessing/PhotonStream.h"
#include "SignalProcessing/ElectricPulse.h"
#include "Core/Random/Random.h"
#include "Core/SimulationTruth.h"
using std::vector;

vector<vector<SignalProcessing::ElectricPulse>> create_photon_stream(
    const uint number_of_channels,
    const float pulse_rate_per_slice,
    const float exposure_time,
    const uint seed) {

    Random::Mt19937 prng(seed);

    vector<vector<SignalProcessing::ElectricPulse>> photon_stream;
    for(uint i=0; i<number_of_channels; i++) {
        vector<SignalProcessing::ElectricPulse> pulses_in_channel;

        float t = 0.0;
        while(true) {

            const float t_next = prng.expovariate(pulse_rate_per_slice);
            if(t + t_next > exposure_time) {
                break;
            }else{
                t += t_next;
                SignalProcessing::ElectricPulse pulse;
                pulse.arrival_time = t;
                pulse.simulation_truth_id = (int)(1000*prng.uniform());
                pulses_in_channel.push_back(pulse);
            }
        } 
        photon_stream.push_back(pulses_in_channel);
    }

    return photon_stream;
}

void expect_eq(
    SignalProcessing::PhotonStream::Stream &A, 
    SignalProcessing::PhotonStream::Stream &B,
    bool simulation_truth_eq=true) {

    EXPECT_EQ(A.slice_duration, B.slice_duration);
    ASSERT_EQ(
        A.photon_stream.size(), 
        B.photon_stream.size());
    for(uint c=0; c<A.photon_stream.size(); c++) {

        ASSERT_EQ(
            A.photon_stream.at(c).size(), 
            B.photon_stream.at(c).size());
        for(uint p=0; p<A.photon_stream.at(c).size(); p++) {

            EXPECT_NEAR(
                A.photon_stream.at(c).at(p).arrival_time,
                B.photon_stream.at(c).at(p).arrival_time,
                A.slice_duration
            );

            if (simulation_truth_eq){
                EXPECT_EQ(
                    A.photon_stream.at(c).at(p).simulation_truth_id, 
                    B.photon_stream.at(c).at(p).simulation_truth_id
                );
            }
        }
    }
}

void write_and_read_back(
    const uint number_of_channels,
    const float sample_frequency,
    const float single_pulse_rate,
    const float exposure_time,
    const uint seed
) {
    const float slice_duration = 1.0/sample_frequency;

    SignalProcessing::PhotonStream::Stream stream;
    stream.photon_stream = create_photon_stream(
            number_of_channels,
            single_pulse_rate,
            exposure_time,
            seed);
    stream.slice_duration = slice_duration;

    const string path = "InOut/photon_stream.bin";
    SignalProcessing::PhotonStream::write(
        stream.photon_stream,
        slice_duration,
        path);

    const string truth_path = "InOut/photon_stream_truth.bin";
    SignalProcessing::PhotonStream::write_simulation_truth(
        stream.photon_stream,
        truth_path);

    SignalProcessing::PhotonStream::Stream ps_without_truth = 
        SignalProcessing::PhotonStream::read(path);

    expect_eq(stream, ps_without_truth, false);

    SignalProcessing::PhotonStream::Stream ps_with_truth = 
        SignalProcessing::PhotonStream::read_with_simulation_truth(
            path, 
            truth_path);

    expect_eq(stream, ps_with_truth);
}

class PhotonStreamTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(PhotonStreamTest, write_and_read_back_full_single_pulse_event) {

    const uint number_of_channels = 1337;
    const float sample_frequency = 2e9;
    const float single_pulse_rate = 50e6;
    const float exposure_time = 127.0e-9;
    const uint seed = 0;

    write_and_read_back(
        number_of_channels,
        sample_frequency,
        single_pulse_rate,
        exposure_time,
        seed);
}
//------------------------------------------------------------------------------
TEST_F(PhotonStreamTest, zero_channels) {

    const uint number_of_channels = 0;
    const float sample_frequency = 2e9;
    const float single_pulse_rate = 50e6;
    const float exposure_time = 127.0e-9;
    const uint seed = 0;

    write_and_read_back(
        number_of_channels,
        sample_frequency,
        single_pulse_rate,
        exposure_time,
        seed);
}
//------------------------------------------------------------------------------
TEST_F(PhotonStreamTest, empty_channels) {

    const uint number_of_channels = 1337;
    const float sample_frequency = 2e9;
    const float single_pulse_rate = 0.0;
    const float exposure_time = 127.0e-9;
    const uint seed = 0;

    write_and_read_back(
        number_of_channels,
        sample_frequency,
        single_pulse_rate,
        exposure_time,
        seed);
}
//------------------------------------------------------------------------------
TEST_F(PhotonStreamTest, number_time_slices_too_big) {

    EXPECT_NO_THROW(
        SignalProcessing::PhotonStream::assert_number_time_slices_below_8bit_max(0);
    );

    EXPECT_THROW(
        SignalProcessing::PhotonStream::assert_number_time_slices_below_8bit_max(256);,
        TracerException
    );
}
//------------------------------------------------------------------------------