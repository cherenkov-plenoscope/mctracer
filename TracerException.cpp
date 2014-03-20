#include "TracerException.h"
//----------------------------------------------------------------------
void TracerException::ExceptionPrompt()const{	
	
	std::stringstream out;
	out.str("");
	out << "Default Tracer Exception: ";
	std::cout << out.str();		
}
//----------------------------------------------------------------------
void TracerException::ReportException()const{
	std::stringstream out;
	out.str("");
	out << "Default Exception Report: ";
	std::cout << out.str();		
};
