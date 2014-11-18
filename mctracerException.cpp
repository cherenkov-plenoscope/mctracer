#include "TracerException.h"

TracerException::TracerException(std::string new_message){
	message = new_message;
}

TracerException::TracerException(std::string message, ExceptionType Type){
	this -> message = message;
	this -> Type = Type;
}

ExceptionType TracerException::type()const{
	return Type;
}

const char * TracerException::what () const throw (){
	std::string type = "TracerException:\n";
	return (type + message).c_str();
}

TracerException::TracerException(){
	message = "Empty";
}

BadValue::BadValue(std::string new_message){
	message = new_message;
}