#include "TimeLineAddition.h"

namespace SignalProcessing {

void add_second_to_first_at(
    vector<double> *first, 
    const vector<double> *second, 
    int i
) {
    unsigned int is=i;
    unsigned int wait=0;
    
    if(i<0) {
        wait = -i;
        is = 0;
    }

    unsigned int j = wait;
    while(is<first->size() && j<second->size()) {
        (*first)[is] += (*second)[j];
        j++;
        is++;
    };
}

}//SignalProcessing