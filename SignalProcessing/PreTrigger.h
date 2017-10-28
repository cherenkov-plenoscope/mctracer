//=================================
// include guard
#ifndef __PreSumTrigger_H_INCLUDED__
#define __PreSumTrigger_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <vector>

namespace SignalProcessing {
	namespace PreTrigger {

struct Config {
	double time_window;
	unsigned int threshold;
	Config();
};

bool might_trigger(
	const std::vector<double> *arrival_moments,
	const Config &config
);
		
	}//PreTrigger
}//SignalProcessing
#endif // __PreSumTrigger_H_INCLUDED__