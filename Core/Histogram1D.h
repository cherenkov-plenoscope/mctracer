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
using std::string;
using std::vector;
//==============================================================================
class Histogram1D {

	vector<double> bin_edges;
public:
	vector<uint> bins;
	uint number_of_samples = 0;
	uint underflow_bin = 0;
	uint overflow_bin = 0;

	Histogram1D(
		const vector<double> &samples, 
		const vector<double> &_bin_edges
	);
	string str()const;
	double mode()const;
	uint arg_max()const;
private:

	void init_bins_to_zero();
	void assert_at_least_two_bin_edges()const;
	void fill_in(const double sample);
};
#endif // __Histogram1D_H_INCLUDED__