//==============================================================================
// include guard
#ifndef __VECTORTOOLS_H_INCLUDED__
#define __VECTORTOOLS_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort
#include <utility>
#include "TracerException.h"
#include "Tools/StringTools.h"

namespace VectorTools {

	enum BoundaryMode {ZERO, CLOSEST, STRICT};

	double interpolate_in_arg_val_at(
		std::vector<double> &x,
		std::vector<double> &y,
		double x_at
	) {
		
	}

	void sort_function_arguments();
	static bool comp_upp( const double A, const std::pair<double,double> B );
	
	double interpolate_linear(
		const std::pair<double,double> p0, 
		const std::pair<double,double> p1, 
		const double x
	)const;
	double at_BoundaryMode_CLOSEST(double func_arg)const;
	double at_BoundaryMode_STRICT(double func_arg)const;
	double at_BoundaryMode_ZERO(double func_arg)const;
	std::vector< std::pair<double,double> >::const_iterator get_upper_bound(double func_arg)const;
	std::string get_boundary_mode_string(BoundaryMode m)const;
};
#endif // __VECTORTOOLS_H_INCLUDED__
