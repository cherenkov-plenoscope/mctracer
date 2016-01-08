//==============================================================================
// include guard
#ifndef __Histogram1D_H_INCLUDED__
#define __Histogram1D_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include <vector>
#include <stdint.h>
#include <iostream>
//==============================================================================
class Histogram1D {

	std::vector<double> bins;
public:
	uint sample_size = 0;
	std::vector<uint> hist;
	uint underflow_bin = 0;
	uint overflow_bin = 0;

	Histogram1D(
		const std::vector<double> &samples, 
		const std::vector<double> &_bins
	);
private:

	std::vector<double>::const_iterator get_upper_bound_bin(
		const double sample
	)const;
	void init_hist();
	void fill_in(const double sample);
	void sort_bins();
};
#endif // __Histogram1D_H_INCLUDED__