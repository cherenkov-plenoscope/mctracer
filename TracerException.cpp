#include "TracerException.h"
//----------------------------------------------------------------------
void TracerException::ExceptionPrompt()const{	
	
	std::stringstream out;
	out.str("");
	out << "Tracer Exception:\n";
	std::cout << out.str();		
}
//----------------------------------------------------------------------
void TracerException::ReportException()const{
	std::stringstream out;
	out.str("");
	out << "Default Exception Report: ";
	std::cout << out.str();		
};
//----------------------------------------------------------------------
BadValue::BadValue(const std::string sit, const std::string nam){
	name_of_bad_value = nam;
	situation = sit;
	additional_information = false;
}
//----------------------------------------------------------------------
BadValue::BadValue(const std::string sit, const std::string nam,const std::string i){
	name_of_bad_value = nam;
	situation = sit;
	info = i;
	additional_information = true;
}
//----------------------------------------------------------------------	
void BadValue::ReportException()const{ 
		

	ExceptionPrompt();
	
	std::stringstream out;	
	out.str("");

	out << "Bad Value: " << situation << "\n";
	out << "Initialzing " << name_of_bad_value << " is not possible. \n";

	if(additional_information){
		out << info << "\n";
	}

	std::cout << out.str() << std::endl;
}	
	