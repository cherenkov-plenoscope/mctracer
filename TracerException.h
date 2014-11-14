//=================================
// include guard
#ifndef __TRACEREXCEPTION_H_INCLUDED__
#define __TRACEREXCEPTION_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
//------------------------------------------------------------------------------
class TracerException :public std::exception{
protected:
	std::string message = "";
public:

	TracerException(std::string new_message);
	TracerException();

	const char * what () const throw ();
};

class BadValue :public TracerException{
public:
	BadValue(std::string new_message);
};

#endif // __TRACEREXCEPTION_H_INCLUDED__