#include "TracerException.h"
//------------------------------------------------------------------------------
TracerException::TracerException(){
	message = "Empty";
}
//------------------------------------------------------------------------------
TracerException::TracerException(std::string message){
	this->message = message;
}
//------------------------------------------------------------------------------
TracerException::TracerException(std::string message, ExceptionType Type){
	this->message = message;
	this->Type = Type;
}
//------------------------------------------------------------------------------
ExceptionType TracerException::type()const{
	return Type;
}
//------------------------------------------------------------------------------
const char* TracerException::what()const noexcept {

	std::string out = get_full_message_print();
	char* charrout = new char[out.length() + 1];
	std::strcpy(charrout, out.c_str());
	return charrout;
}
//------------------------------------------------------------------------------
std::string TracerException::get_full_message_print()const {
	std::stringstream out;
	out << " ___TracerException___\n" << 
		StringTools::place_first_infront_of_each_new_line_of_second(
			"| ",
			message
		);
	//out << "\n";
	out << "|_____________________\n";
	return out.str();
}
//------------------------------------------------------------------------------
BadValue::BadValue(std::string message){
	this->message = message;
}
//------------------------------------------------------------------------------