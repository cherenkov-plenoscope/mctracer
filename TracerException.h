//=================================
// include guard
#ifndef __TRACEREXCEPTION_H_INCLUDED__
#define __TRACEREXCEPTION_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <stdexcept>

//------------------------------------------------------------------------------
class TracerException :public std::exception{
protected:

	std::string message = "";
	const char* str2chararray(const std::string text)const;
	TracerException();
public:

	TracerException(std::string message);
	const char * what () const throw();
	static const std::string compile_time;
private:

	std::string get_full_message_print()const;
	static std::string get_compile_time();
};
#endif // __TRACEREXCEPTION_H_INCLUDED__