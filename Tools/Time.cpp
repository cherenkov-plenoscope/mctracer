#include "Time.h"
#include <ctime>
#include <ratio>
#include <sstream>
#include <iomanip>

namespace Time {
	Point now() {
		return std::chrono::high_resolution_clock::now();
	}

	double periode(const Point &t1, const Point &t2) {
		// return in SI unit Secoconds
		return 0.001*
			std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
	}

	StopPeriode::StopPeriode() {
		t1 = Time::now();
	}

	double StopPeriode::stop()const {
		std::chrono::high_resolution_clock::time_point t2 = Time::now();
		return 0.001*std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count(); 
	}

	StopWatch::StopWatch(const string what_to_time): 
		stop_periode(StopPeriode()),
		what(what_to_time)
	{}

	void StopWatch::stop() {
		std::cout << what << ": " << stop_periode.stop() << "s\n";
	}

	Stamp::Stamp() {
		update_now();
	}

	void Stamp::update_now() {
		initialization_time = time(0);
		now = localtime(&initialization_time);		
	}

	string Stamp::str()const {
		std::stringstream out; 
		out << "time stamp: ";
		out <<yyyy()<<"."<<mm()<<"."<<dd()<<" "<<HH()<<":"<<MM()<<":"<<SS();
		return out.str();
	}

	string Stamp::yyyy()const {
		return fill_zeros_to_value_for_width(now->tm_year+1900, 4);
	}

	string Stamp::mm()const {
		return fill_zeros_to_value_for_width(now->tm_mon+1, 2);
	}

	string Stamp::dd()const {
		return fill_zeros_to_value_for_width(now->tm_mday, 2);
	}

	string Stamp::HH()const {
		return fill_zeros_to_value_for_width(now->tm_hour, 2);
	}

	string Stamp::MM()const {
		return fill_zeros_to_value_for_width(now->tm_min, 2);
	}

	string Stamp::SS()const {
		return fill_zeros_to_value_for_width(now->tm_sec, 2);
	}

	string Stamp::fill_zeros_to_value_for_width(
		const uint value, const uint width
	)const {
		std::stringstream out;
		out << std::setfill('0') << std::setw(width) << value;
		return out.str();
	}
}//Time