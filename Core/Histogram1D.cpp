#include "Histogram1D.h"
#include <algorithm>
#include "Tools/StringTools.h"
#include <iomanip> 
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
		bins.clear();
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
	bins.reserve(bin_count);
	
	for(uint i=0; i<bin_count; i++)
		bins.push_back(0);
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
	bins[bin_index]++;
}
//------------------------------------------------------------------------------
std::string Histogram1D::get_print()const {

	std::stringstream out;
	out.precision(2);

	uint max_cols = 40;
	uint max_bin_count = 0;
	for(uint bin: bins) if(bin > max_bin_count) max_bin_count = bin;

	double scaling = double(max_cols)/double(max_bin_count);
	
	uint bin_number_cols = ceil(log10(bins.size()));
	if(bin_number_cols<2) bin_number_cols = 2;

	out << "underflow " << underflow_bin << "\n";
	for(uint i=0; i<bins.size(); i++) {
		out << std::setw(bin_number_cols) << i << " ";
		out << "[" << std::setw(8) << bin_edges[i] << ", ";
		out << std::setw(8) << bin_edges[i+1] << "] ";
		out << StringTools::repeat_multiple_times("|",uint(scaling*bins[i]));
		out << " " << bins[i] << "\n";
	}
	out << "overflow  " << overflow_bin << "\n";

	return out.str();	
}