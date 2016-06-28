//=================================
// include guard
#ifndef __Time_H_INCLUDED__
#define __Time_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <chrono>
#include <string>
#include <map>
#include <vector>
using std::string;
using std::vector;

//=================================
namespace Time {
	#define Point std::chrono::high_resolution_clock::time_point
	Point now();
	double periode(const Point &t1, const Point &t2); 
	//--------------------------------------------------------------------------
	class StopPeriode {
		const string what;
		Point t1;
	public:
		StopPeriode();
		double stop()const;
	};
	//--------------------------------------------------------------------------
	class StopWatch {
		StopPeriode stop_periode;
		const string what;
	public:

		StopWatch(const string what_to_time);
		void stop();
	};
	//--------------------------------------------------------------------------
	class Stamp {

		std::time_t initialization_time;
		struct std::tm *now;
	public:

		Stamp();
		void update_now();
		string get_print()const;
		string yyyy()const;
		string mm()const;
		string dd()const;
		string HH()const;
		string MM()const;
		string SS()const;
	private:
		string fill_zeros_to_value_for_width(
			const uint value, 
			const uint width
		)const;
	};
}
#endif // __Time_H_INCLUDED__