//=================================
// include guard
#ifndef __PreSumTrigger_H_INCLUDED__
#define __PreSumTrigger_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>

namespace PreTrigger {

	struct Config {
		double time_window;
		uint threshold;
		Config();
	};

	bool might_trigger(
		const std::vector<double> *arrival_moments,
		const Config &config
	);
}
#endif // __PreSumTrigger_H_INCLUDED__