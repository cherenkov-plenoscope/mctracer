//=================================
// include guard
#ifndef __Histogramm_H_INCLUDED__
#define __Histogramm_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies	  
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <fstream>
#include "TracerException.h"

class Histogramm{

	uint *bins;

	int nbins;
	double start;
	double end;
	double range;
	double inv_range;
public:
	Histogramm(const double _start, const double _end, const uint _nbins) {
		nbins = _nbins;
		start = _start;
		end = _end;
		range = end - start;
		inv_range = 1.0/range;
		bins = new uint [nbins];
		for(int i=0; i<nbins; i++)
			bins[i] = 0;	 
	}

	void fill_in(const double entry) {
		const int pos = int(floor((entry - start)*inv_range*nbins));
		//std::cout << "fill in " << entry*1e9 << "nm to bin " << pos;
		if(pos>=0 && pos < nbins) {
			bins[pos]++;
		//	std::cout <<" + ";
		}
		//std::cout <<"\n";
	}

	void fill_in(const std::vector<double> vec) {
		for(double entry : vec) {
			fill_in(entry);
		}
	}

	std::string get_print()const {
		std::stringstream out;
		for(int i=0; i<nbins; i++)
			out << bins[i] << "\n";
		return out.str();
	}
};

#endif // __Histogramm_H_INCLUDED__