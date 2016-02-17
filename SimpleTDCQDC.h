//=================================
// include guard
#ifndef __SimpleTDCQDC_H_INCLUDED__
#define __SimpleTDCQDC_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <vector>
using std::vector;

namespace DigitalTDCQDC{

    struct Config {
        double integration_time;
    };

    struct TimeAndCount {
        double time;
        double count;
    };

    TimeAndCount find(const Config config, const vector<double> &arrival_pipeline);
}//TdC QdC
#endif // __SimpleTDCQDC_H_INCLUDED__