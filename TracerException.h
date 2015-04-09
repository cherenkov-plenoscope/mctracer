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
#include <cstring>
#include <sstream>
#include "Tools/StringTools.h"

enum ExceptionType {
	NOT_SPECIFIED,
	EMPTY_NAME_OF_FRAME,
	DELIMITER_SYMBOL_IN_NAME_OF_FRAME,
	WHITE_SPACE_IN_NAME_OF_FRAME,
	
	CAN_NOT_OPEN_MMCS_FILE,
	FILE_SIZE_IS_NOT_MULTIPLE_OF_MMCS_BLOCKSIZE,
	CAN_NOT_HANDLE_GZIP_FILE,
	CAN_NOT_PARSE_MMCS_EVENT_HEADER_FROM_NON_EVTH_SUB_BLOCK,

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

	TracerException(std::string message);
	TracerException(std::string message, ExceptionType Type);
	TracerException();
	ExceptionType type()const;
	const char * what () const throw();
private:
	std::string get_full_message_print()const;
};

class BadValue :public TracerException{
public:
	BadValue(std::string message);
};

#endif // __TRACEREXCEPTION_H_INCLUDED__