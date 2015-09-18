//==============================================================================
// include guard
#ifndef __FUNC1D_H_INCLUDED__
#define __FUNC1D_H_INCLUDED__

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

class Func1D {

	std::vector<std::pair<double,double>> func;
public:

	Func1D(std::vector<std::vector<double>> two_column_xy);
	double interploate_value_at_argument(const double argument)const;
	double get_weighted_mean_of_value()const;
	std::string get_print()const;
	//double at(const double x)const;

private:
	//static bool comp( const double A, const std::vector<double> row );
	//std::vector<std::vector<double>>::const_iterator upper_bound(double x)const;
	std::string exception_header()const;
	void sort_function_arguments();
	void assert_no_duplicate_argument_on_sorted_arguments()const;
	static bool comp_upp( const double A, const std::pair<double,double> B );
	double interpolate_linear(
		const std::pair<double,double> p0, 
		const std::pair<double,double> p1, 
		const double x
	)const;
	double at_with_Boundary_Mode_STRICT(const double func_arg)const;
	std::vector< std::pair<double,double> >::const_iterator get_upper_bound(double func_arg)const;
	double get_integral_over_value()const;
	double get_range_of_argument()const;
	void assert_func_is_not_empty()const;
};
#endif // __FUNC1D_H_INCLUDED__
