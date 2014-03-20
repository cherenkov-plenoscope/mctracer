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

//=================================
class TracerException{
	public:
	virtual void ReportException()const;
	virtual void ExceptionPrompt()const;
};
#endif // __TRACEREXCEPTION_H_INCLUDED__
