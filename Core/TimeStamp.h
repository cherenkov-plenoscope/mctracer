//=================================
// include guard
#ifndef __TIMESTAMP_H_INCLUDED__
#define __TIMESTAMP_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
//=================================
class TimeStamp {

	std::time_t initialization_time;
	struct std::tm *now;
public:

	TimeStamp();
	void update_now();
	std::string get_print()const;
	std::string yyyy()const;
	std::string mm()const;
	std::string dd()const;
	std::string HH()const;
	std::string MM()const;
	std::string SS()const;
private:
	std::string fill_zeros_to_value_for_width(
		const uint value, 
		const uint width
	)const;
};
#endif // __TIMESTAMP_H_INCLUDED__