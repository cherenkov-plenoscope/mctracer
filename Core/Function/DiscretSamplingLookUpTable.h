// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_DISCRETSAMPLINGLOOKUPTABLE_H_
#define CORE_FUNCTION_DISCRETSAMPLINGLOOKUPTABLE_H_

#include <vector>
#include "Core/Function/Limits.h"
#include "Core/Function/Func1D.h"

namespace Function {
namespace DiscretSampling {

struct Config {
    double slice_width;
    unsigned int samples_per_slice;
    Config();
};

class LookUpTable {
    Config config;
    const Func1D* func;
    std::vector<std::vector<double>> table;
    unsigned int number_of_slices;
    double step_width_in_slice;

 public:
    LookUpTable(const Func1D* func, const Config config);
    unsigned int size()const;
    const std::vector<double>* at(const double slice_offset)const;

 private:
    void fill_table();
};

}  // namespace DiscretSampling
}  // namespace Function

#endif  // CORE_FUNCTION_DISCRETSAMPLINGLOOKUPTABLE_H_
