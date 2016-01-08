//=================================
// include guard
#ifndef __TOOLS_H_INCLUDED__
#define __TOOLS_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "math.h"	  
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>

double Deg2Rad(double angle_in_Deg);
double Rad2Deg(double angle_in_Rad);
bool is_odd(const int n);
bool is_even(const int n);

double StrToDouble(std::string text_to_parse);
bool StrToBool(std::string text_to_parse);
int StrToInt(std::string text_to_parse);
namespace numeric {
	double mean(const std::vector<double> &vec);
	double stddev(const std::vector<double> &vec);
	std::vector<double> linspace(double begin, double end, uint steps);
}
double get_mean_along_column(const std::vector<std::vector<double>> &table, const uint column);
std::string get_print(const std::vector<std::vector<double>> &table);

#endif // __TOOLS_H_INCLUDED__