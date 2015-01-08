#include "TracerException.h"

TracerException::TracerException(std::string message){
	this->message = message;
}

TracerException::TracerException(std::string message, ExceptionType Type){
	this->message = message;
	this->Type = Type;
}

ExceptionType TracerException::type()const{
	return Type;
}

const char * TracerException::what () const noexcept {
	std::stringstream out;
	out << "TracerException:\n" << message;
	return out.str().c_str();
}

TracerException::TracerException(){
	message = "Empty";
}

BadValue::BadValue(std::string message){
	this->message = message;
}