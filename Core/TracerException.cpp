#include "Core/TracerException.h"
#include <cstring>
#include <sstream>
#include "Tools/StringTools.h"
//------------------------------------------------------------------------------
const string TracerException::compile_time = 
	TracerException::get_compile_time();
//------------------------------------------------------------------------------
TracerException::TracerException(){
	message = "Empty";
}
//------------------------------------------------------------------------------
TracerException::TracerException(const string message){
	this->message = message;
}
//------------------------------------------------------------------------------
const char* TracerException::what()const noexcept {
	return str2chararray(get_full_message_print());
}
//------------------------------------------------------------------------------
string TracerException::get_full_message_print()const {
	std::stringstream out;
	out << "\n";
	out << " ___TracerException___\n";
	out << "| compiled on: " << compile_time << "\n";
	out << 	StringTools::place_first_infront_of_each_new_line_of_second(
		   		"| ",
				message
			);
	out << "|_____________________\n";
	out << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
string TracerException::get_compile_time() {
	std::stringstream timestamp;
	timestamp << __DATE__ << ", " << __TIME__;
	return timestamp.str();
}
//------------------------------------------------------------------------------
const char* TracerException::str2chararray(const string text)const {
	char* charrout = new char[text.length() + 1];
	std::strcpy(charrout, text.c_str());
	return charrout;
}