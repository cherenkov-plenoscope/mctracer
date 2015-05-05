#include "TimeStamp.h"
//------------------------------------------------------------------------------
TimeStamp::TimeStamp() {
	update_now();
}
//------------------------------------------------------------------------------
void TimeStamp::update_now() {
	initialization_time = time(0);
	now = localtime(&initialization_time);		
}
//------------------------------------------------------------------------------
std::string TimeStamp::get_print()const {
	std::stringstream out; 
	out << "time stamp: ";
	out <<yyyy()<<"."<<mm()<<"."<<dd()<<" "<<HH()<<":"<<MM()<<":"<<SS();
	return out.str();
}
//------------------------------------------------------------------------------
std::string TimeStamp::yyyy()const {
	return fill_zeros_to_value_for_width(now->tm_year+1900, 4);
}
//------------------------------------------------------------------------------
std::string TimeStamp::mm()const {
	return fill_zeros_to_value_for_width(now->tm_mon+1, 2);
}
//------------------------------------------------------------------------------
std::string TimeStamp::dd()const {
	return fill_zeros_to_value_for_width(now->tm_mday, 2);
}
//------------------------------------------------------------------------------
std::string TimeStamp::HH()const {
	return fill_zeros_to_value_for_width(now->tm_hour, 2);
}
//------------------------------------------------------------------------------
std::string TimeStamp::MM()const {
	return fill_zeros_to_value_for_width(now->tm_min, 2);
}
//------------------------------------------------------------------------------
std::string TimeStamp::SS()const {
	return fill_zeros_to_value_for_width(now->tm_sec, 2);
}
//------------------------------------------------------------------------------
std::string TimeStamp::fill_zeros_to_value_for_width(
	const uint value, 
	const uint width
)const {
	std::stringstream out;
	out << std::setfill('0') << std::setw(width) << value;
	return out.str();
}