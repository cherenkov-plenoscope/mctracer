//==============================================================================
// include guard
#ifndef __Histogram1D_H_INCLUDED__
#define __Histogram1D_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include <vector>
#include <string>

//==============================================================================
class Histogram1D {

	std::vector<double> bin_edges;
public:
	std::vector<unsigned int> bins;
	unsigned int number_of_samples = 0;
	unsigned int underflow_bin = 0;
	unsigned int overflow_bin = 0;

	Histogram1D(
		const std::vector<double> &samples, 
		const std::vector<double> &_bin_edges
	);
	std::string str()const;
	double mode()const;
	unsigned int arg_max()const;
private:

	void init_bins_to_zero();
	void assert_at_least_two_bin_edges()const;
	void fill_in(const double sample);
};
#endif // __Histogram1D_H_INCLUDED__