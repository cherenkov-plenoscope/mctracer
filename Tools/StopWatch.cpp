#include "StopWatch.h"
#include <ctime>
#include <ratio>

StopWatch::StopWatch(const string what_to_time): what(what_to_time) {
	t1 = now();
}

void StopWatch::stop() {
	t2 = now();
	std::cout << what << ": "
	<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() 
	<< "ms\n";
}

std::chrono::high_resolution_clock::time_point StopWatch::now()const {
	return std::chrono::high_resolution_clock::now();
}