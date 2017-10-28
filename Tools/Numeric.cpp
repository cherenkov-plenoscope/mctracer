#include "Numeric.h"
#include <math.h>
using std::vector;

//------------------------------------------------------------------------------
double Numeric::mean(const vector<double> &vec) {
	double sum = 0;
	for(double val: vec) sum = sum + val;
	return sum/vec.size();
}
//------------------------------------------------------------------------------
double Numeric::stddev(const vector<double> &vec) {
	double mean = Numeric::mean(vec);
    double s = 0.0;
    for(double val: vec) s = s + (val - mean)*(val - mean);
    return sqrt(s/vec.size());
}
//------------------------------------------------------------------------------
vector<double> Numeric::linspace(double begin, double end, unsigned int steps) {
		
	vector<double> lin;
	lin.reserve(steps);

	double range = end - begin; 
	double step = range/double(steps-1);

	for(unsigned int i=0; i<steps; i++)
		lin.push_back(begin + step*i);

	return lin;
}
//------------------------------------------------------------------------------