//=================================
// include guard
#ifndef __TOOLS_NUMERIC_H_INCLUDED__
#define __TOOLS_NUMERIC_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <vector>

namespace Numeric {
    double mean(const std::vector<double> &vec);
    double stddev(const std::vector<double> &vec);
    std::vector<double> linspace(double begin, double end, unsigned int steps);
}

#endif // __TOOLS_NUMERIC_H_INCLUDED__