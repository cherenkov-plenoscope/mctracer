#include "Histogram1D.h"
#include <algorithm>
//------------------------------------------------------------------------------
Histogram1D::Histogram1D(
	const std::vector<double> &samples, 
	const std::vector<double> &_bins
):bins(_bins) {

	sort_bins();
	init_hist();
	for(double sample: samples)
		fill_in(sample);

	sample_size = samples.size();
}
//------------------------------------------------------------------------------
void Histogram1D::sort_bins() {
	std::sort(bins.begin(), bins.end());
}
//------------------------------------------------------------------------------
std::vector<double>::const_iterator Histogram1D::get_upper_bound_bin(
	const double sample
)const {
	return std::upper_bound(bins.begin(), bins.end(), sample);		
}
//------------------------------------------------------------------------------
void Histogram1D::init_hist() {

	uint bin_count = bins.size() - 1;
	hist.reserve(bin_count);
	
	for(uint i=0; i<bin_count; i++)
		hist.push_back(0);
}
//------------------------------------------------------------------------------
void Histogram1D::fill_in(const double sample) {

	std::vector<double>::const_iterator up_bin = get_upper_bound_bin(sample);

	if(up_bin==bins.begin()) {
		underflow_bin++;
		return;
	}

	if(up_bin==bins.end() && *up_bin < sample) {
		overflow_bin++;
		return;
	}

	uint bin_index = (up_bin - bins.begin());
	hist[bin_index]++;
}