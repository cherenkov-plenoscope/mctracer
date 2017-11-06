//=================================
// include guard
#ifndef __Time_H_INCLUDED__
#define __Time_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <chrono>
#include <string>

//=================================
namespace Time {
	#define Point std::chrono::high_resolution_clock::time_point
	Point now();
	double periode(const Point &t1, const Point &t2); 
	//--------------------------------------------------------------------------
	class StopPeriode {
		const std::string what;
		Point t1;
	public:
		StopPeriode();
		double stop()const;
	};
	//--------------------------------------------------------------------------
	class StopWatch {
		StopPeriode stop_periode;
		const std::string what;
	public:

		StopWatch(const std::string what_to_time);
		void stop();
	};
	//--------------------------------------------------------------------------
	class Stamp {

		std::time_t initialization_time;
		struct std::tm *now;
	public:

		Stamp();
		void update_now();
		std::string str()const;
		std::string yyyy()const;
		std::string mm()const;
		std::string dd()const;
		std::string HH()const;
		std::string MM()const;
		std::string SS()const;
	private:
		std::string fill_zeros_to_value_for_width(
			const unsigned int value, 
			const unsigned int width
		)const;
	};
}
#endif // __Time_H_INCLUDED__