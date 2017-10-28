//=================================
// include guard
#ifndef __FunctionDiscretSamplingLookUpTable_H_INCLUDED__
#define __FunctionDiscretSamplingLookUpTable_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LimitsFunction.h"
#include "Func1DFunction.h"
#include <vector>

//------------------------------------------------------------------------------
namespace Function {
	namespace DiscretSampling {

struct Config {

	double slice_width;
	uint samples_per_slice;
	Config();
};

class LookUpTable {

	Config config;
	const Func1D* func;

	std::vector<std::vector<double>> table;

	uint number_of_slices;
	double step_width_in_slice;
public: 

	LookUpTable(const Func1D* func, const Config config);
	uint size()const;
	const std::vector<double>* at(const double slice_offset)const;
private:

	void fill_table();
};

	}// DiscretSampling
}// Function
#endif // __FunctionDiscretSamplingLookUpTable_H_INCLUDED__