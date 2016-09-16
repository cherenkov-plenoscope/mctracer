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
using std::string;

//------------------------------------------------------------------------------
class TracerException :public std::exception{
protected:

	string message = "";
	const char* str2chararray(const string text)const;
	TracerException();
public:

	TracerException(const string message);
	const char * what () const throw();
	static const string compile_time;
private:

	string get_full_message_print()const;
	static string get_compile_time();
};
#endif // __TRACEREXCEPTION_H_INCLUDED__