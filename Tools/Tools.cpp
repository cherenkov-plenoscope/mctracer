#include "Tools.h"
//------------------------------------------------------------------------------
double Deg2Rad(double angle_in_Deg) {
	return (M_PI*angle_in_Deg)/180.0;
}
//------------------------------------------------------------------------------
double Rad2Deg(double angle_in_Rad) {
	return (angle_in_Rad/M_PI)*180.0;
}
//------------------------------------------------------------------------------
namespace UserInteraction {

	std::string input(const std::string request) {
		std::cout << request;
		std::string user_input;
		std::cin  >> user_input;	
		return user_input;
	}
    //--------------------------------------------------------------------------
	void print_welcome_screen() {

		std::stringstream compile_time;
		compile_time << __DATE__ << ", " << __TIME__;

		std::stringstream out;
		//               1         2         3         4         5         6
		//      123456789012345678901234567890123456789012345678901234567890
		out << " ___________________________________________________________ \n";
		out << "|                                                           |\n";
		out << "|                 welcome to MCtracer                       |\n";
		out << "|                                                           |\n";
		out << "|      high performance raytracing tool to design and       |\n";
		out << "|     investigate optical devices for particle physics      |\n";
		out << "|                                                           |\n";
		out << "|     Author: Sebastian Mueller              year 2013      |\n";
		out << "|     compiled on: " << compile_time.str() << "\n";	
		out << "|___________________________________________________________|\n";
		std::cout << out.str();	
	}
	//--------------------------------------------------------------------------
	void ClearScreen() {
		std::cout << std::string( 5, '\n' );
	}
	//--------------------------------------------------------------------------
	std::string parse_config_file_path(int number_of_arguments, char* arguments[]) {

		if(number_of_arguments != 2) {
			std::stringstream info;
			info << "Wrong number of input arguments, expected 1, but actual: ";
			info << number_of_arguments-1 << ".\n";
			throw TracerException(info.str());
		}

		std::string config_path(arguments[1]);
	    return config_path;
	}
	//--------------------------------------------------------------------------
	bool is_Escape_key(const int user_input_key) {
		return user_input_key == 27;
	}
}
//------------------------------------------------------------------------------
namespace AssertionTools {
	//--------------------------------------------------------------------------
	void value_with_name_is_greater_zero_given_context(
		const double value, 
		const std::string name, 
		const std::string context
	) {
		if(value <= 0.0) {
			std::stringstream out;
			out << "Expected " << name << " > 0.0, but actual ";
			out << name << " = " << value << ". " << context; 
			throw TracerException(out.str());
		}
	}
	//--------------------------------------------------------------------------
	void value_with_name_is_in_min_max_given_context(
		const double value, 
		const std::string name,
		const double min,
		const double max, 
		const std::string context
	) {
		if(value < min || value > max) {
			std::stringstream out;
			out << "Expected ";
			out << min << " < " << name << " < " << max << ", but actual ";
			out << name << " = " << value << ". " << context; 
			throw TracerException(out.str());
		}
	}
	//--------------------------------------------------------------------------
	void text_with_name_is_not_empty_given_context(
		const std::string text_to_check,
		const std::string name_of_text,
		const std::string context
	) {

		if(text_to_check.empty()){
			std::stringstream info;
			info << "Expected " << name_of_text << " not to be empty, ";
			info << "but actually it is empty. ";
			info << context;
			throw TracerException(info.str());
		}		
	}
	//--------------------------------------------------------------------------
	void text_with_name_has_no_whitespaces_given_context(
		const std::string text_to_check,
		const std::string name_of_text,
		const std::string context
	) {
		uint pos = 0;
		for(auto single_character : text_to_check) {
			if(isspace(single_character)) {
				std::stringstream info;
				info << "Expected " << name_of_text << " ";
				info << "to have no whitespaces, but actual the char at pos ";
				info << pos << " is a whitespace: '";
				info << text_to_check << "'. " << context;
				throw TracerException(info.str());			
			}
			pos++;
		}
	}
	//--------------------------------------------------------------------------
	void text_with_name_has_no_specific_char_given_context(
		const std::string text_to_check,
		const std::string name_of_text,
		const char specific_char,
		const std::string context
	) {
		if(StringTools::string_contains_char(text_to_check, specific_char)) {
			std::stringstream info;
			info << "Expected " << name_of_text << " to not contain any char ";
			info << "'" << specific_char << "', but actual: ";
			info << name_of_text << " = '" << text_to_check << "'. " << context;
			throw TracerException(info.str());		
		}	
	}
}

double pedantic_strtod(std::string text_to_parse) {

	if(text_to_parse.compare("") == 0){
		std::stringstream info;
		info << "pedantic_strtod:\n";
		info << "String is empty.";
		throw TracerException(info.str());
	}
	
	char * e;
	double number_parsed_in = std::strtod(text_to_parse.c_str(), &e);

	if (*e != 0){
		std::stringstream info;
		info << "pedantic_strtod:\n";
		info << "Can not convert string to floating point number.";
		throw TracerException(info.str());
	}

	return number_parsed_in;
}