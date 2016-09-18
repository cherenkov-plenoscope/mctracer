//=================================
// include guard
#ifndef __ElectricPulse_H_INCLUDED__
#define __ElectricPulse_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
namespace SignalProcessing {

struct ElectricPulse {
    double arrival_time;
    int simulation_truth_id;
    ElectricPulse();
    ElectricPulse(
        const double arrival_time,
        const int simulation_truth_id
    );
};

}//SignalProcessing
#endif // __ElectricPulse_H_INCLUDED__