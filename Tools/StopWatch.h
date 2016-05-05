//=================================
// include guard
#ifndef __StopWatch_H_INCLUDED__
#define __StopWatch_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <chrono>
#include <string>
using std::string;

//=================================
class StopWatch {
	const string what;

	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;
public:

	StopWatch(const string what_to_time);
	void stop();
private:

	std::chrono::high_resolution_clock::time_point now()const;
};
#endif // __StopWatch_H_INCLUDED__