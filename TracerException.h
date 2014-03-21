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
//=================================
class BadValue : public TracerException{
	public:
	
	BadValue(const std::string sit, const std::string nam);
	BadValue(const std::string sit, const std::string nam,const std::string i);
	void ReportException()const;
	
	private:
	std::string situation;
	std::string name_of_bad_value;
	std::string info;
	bool additional_information;
};
#endif // __TRACEREXCEPTION_H_INCLUDED__
