#include "Histogram1D.h"
#include <algorithm>
//------------------------------------------------------------------------------
Histogram1D::Histogram1D(
	const std::vector<double> &samples, 
	const std::vector<double> &_bin_edges
):bin_edges(_bin_edges) {

	if(bin_edges.size() != 0) {
		sort_bins();
		init_hist();
		for(double sample: samples)
			fill_in(sample);
		number_of_samples = samples.size();
	}else{
		hist.clear();
		number_of_samples = samples.size();
		overflow_bin = samples.size();
	}
}
//------------------------------------------------------------------------------
void Histogram1D::sort_bins() {
	std::sort(bin_edges.begin(), bin_edges.end());
}
//------------------------------------------------------------------------------
std::vector<double>::const_iterator Histogram1D::get_upper_bound_bin_edge(
	const double sample
)const {
	return std::upper_bound(bin_edges.begin(), bin_edges.end(), sample);		
}
//------------------------------------------------------------------------------
void Histogram1D::init_hist() {

	uint bin_count = bin_edges.size() - 1;
	hist.reserve(bin_count);
	
	for(uint i=0; i<bin_count; i++)
		hist.push_back(0);
}
//------------------------------------------------------------------------------
void Histogram1D::fill_in(const double sample) {

	std::vector<double>::const_iterator up_bin_edge = 
		get_upper_bound_bin_edge(sample);

	if(up_bin_edge == bin_edges.begin()) {
		underflow_bin++;
		return;
	}

	if(up_bin_edge == bin_edges.end() && *up_bin_edge < sample) {
		overflow_bin++;
		return;
	}

	uint bin_index = ((up_bin_edge-1) - bin_edges.begin());
	hist[bin_index]++;
}