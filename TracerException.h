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
#include <string>
#include <sstream>

enum ExceptionType {
	NOT_SPECIFIED,
	EMPTY_NAME_OF_FRAME,
	DELIMITER_SYMBOL_IN_NAME_OF_FRAME,
	WHITE_SPACE_IN_NAME_OF_FRAME,
	
	CAN_NOT_OPEN_MMCS_FILE,
	FILE_SIZE_IS_NOT_MULTIPLE_OF_MMCS_BLOCKSIZE,
	CAN_NOT_HANDLE_GZIP_FILE,

	FINITE_STATE_MASHINE_HAS_DUPLICATE_STATE,
	FINITE_STATE_MASHINE_UNKNOWN_STATE,
	FINITE_STATE_MASHINE_DEFINE_STATE_AFTER_TRANSITION
};

//------------------------------------------------------------------------------
class TracerException :public std::exception{
protected:
	std::string message = "";
	ExceptionType Type = NOT_SPECIFIED;
public:

	TracerException(std::string new_message);
	TracerException(std::string new_message, ExceptionType Type);
	TracerException();
	ExceptionType type()const;
	virtual const char * what () const throw ();
};

class BadValue :public TracerException{
public:
	BadValue(std::string new_message);
};

#endif // __TRACEREXCEPTION_H_INCLUDED__