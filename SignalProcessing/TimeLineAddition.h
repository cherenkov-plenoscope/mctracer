//=================================
// include guard
#ifndef __SignalProcessing_TimeLineAddition_H_INCLUDED__
#define __SignalProcessing_TimeLineAddition_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <vector>

namespace SignalProcessing {

    void add_second_to_first_at(
        std::vector<double> *first, 
        const std::vector<double> *second, 
        int i
    );

}//SignalProcessing
#endif // __SignalProcessing_TimeLineAddition_H_INCLUDED__