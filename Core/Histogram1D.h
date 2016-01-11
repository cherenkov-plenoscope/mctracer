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

	std::vector<double> bin_edges;
public:
	std::vector<uint> hist;
	uint number_of_samples = 0;
	uint underflow_bin = 0;
	uint overflow_bin = 0;

	Histogram1D(
		const std::vector<double> &samples, 
		const std::vector<double> &_bin_edges
	);
private:

	std::vector<double>::const_iterator get_upper_bound_bin_edge(
		const double sample
	)const;
	void init_hist();
	void fill_in(const double sample);
	void sort_bins();
};
#endif // __Histogram1D_H_INCLUDED__