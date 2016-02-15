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
using std::string;
//=================================
class TimeStamp {

	std::time_t initialization_time;
	struct std::tm *now;
public:

	TimeStamp();
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
#endif // __TIMESTAMP_H_INCLUDED__