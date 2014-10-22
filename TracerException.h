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
class TracerException{
	public:
	virtual void ReportException()const;
	virtual void ExceptionPrompt()const;
};
//------------------------------------------------------------------------------
class Info: public TracerException{
	std::string info="";
public:
	Info();
	Info(std::string new_info){ info = new_info;};
	void ReportException()const{
		ExceptionPrompt();
		std::cout << info << std::endl;
	};
};
//------------------------------------------------------------------------------
class FileIoException: public TracerException{
	public:
	FileIoException(const std::string new_filename, const std::string new_info){
		filename = new_filename;
		info = new_info;
	}
	void ReportException()const{

		ExceptionPrompt();

		std::stringstream out;
		out << "File IO exception in file: " << filename << "\n";
		out << "Additional information: " << info << "\n";
		std::cout << out.str();
	}
	private:
	std::string filename;
	std::string info;
};
//------------------------------------------------------------------------------
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
