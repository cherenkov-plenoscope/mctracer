//=================================
// include guard
#ifndef __TOOLS_NUMERIC_H_INCLUDED__
#define __TOOLS_NUMERIC_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <vector>
using std::vector;

namespace Numeric {
    double mean(const vector<double> &vec);
    double stddev(const vector<double> &vec);
    vector<double> linspace(double begin, double end, unsigned int steps);
}

#endif // __TOOLS_NUMERIC_H_INCLUDED__