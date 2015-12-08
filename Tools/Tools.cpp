#include "Tools.h"
#include "Tools/StringTools.h"
//------------------------------------------------------------------------------
double Deg2Rad(double angle_in_Deg) {
	return (M_PI*angle_in_Deg)/180.0;
}
//------------------------------------------------------------------------------
double Rad2Deg(double angle_in_Rad) {
	return (angle_in_Rad/M_PI)*180.0;
}
//------------------------------------------------------------------------------
bool is_odd(const int n) {
	return n%2 != 0;
}
//------------------------------------------------------------------------------
bool is_even(const int n) {
	return !is_odd(n);
}
//------------------------------------------------------------------------------
std::string get_print(const std::vector<std::vector<double>> &table) {

	std::stringstream out;
	out << "\n";
	for(std::vector<double> row : table) {
		for(double entry : row) {
			out << entry << "\t";
		}
		out << "\n";
	}
	
	return out.str();	
}
//------------------------------------------------------------------------------
double get_mean_along_column(const std::vector<std::vector<double>> &table, const uint column) {

	double sum = 0.0;

	for(std::vector<double> row : table)
		sum = sum + row.at(column);	

	double mean = sum/double(table.size());

	return mean;	
}
//------------------------------------------------------------------------------
double StrToDouble(std::string text_to_parse) {

	if(text_to_parse.compare("") == 0){
		std::stringstream info;
		info << "StrToDouble: String is empty.";
		throw TracerException(info.str());
	}
	
	char *e;
	double number_parsed_in = std::strtod(text_to_parse.c_str(), &e);

	if (*e != 0){
		std::stringstream info;
		info << "StrToDouble: ";
		info << "Can not parse '" << text_to_parse << "' to double.";
		throw TracerException(info.str());
	}

	return number_parsed_in;
}
//------------------------------------------------------------------------------
bool StrToBool(std::string text_to_parse) {

	if(text_to_parse.compare("") == 0){
		std::stringstream info;
		info << "StrToBool: String is empty.";
		throw TracerException(info.str());
	}
	
	std::transform(
		text_to_parse.begin(), 
		text_to_parse.end(), 
		text_to_parse.begin(), 
		::tolower
	);

	if(StringTools::is_equal(text_to_parse,"true"))
		return true;
	else if(StringTools::is_equal(text_to_parse,"false"))
		return false;
	else {
		std::stringstream info;
		info << "StrToBool: Can not parse: '" << text_to_parse << "' to bool";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
int StrToInt(std::string text_to_parse) {

	if(text_to_parse.compare("") == 0){
		std::stringstream info;
		info << "StrToInt: String is empty.";
		throw TracerException(info.str());
	}
	
	const int dezimal_base = 10;
	char *e;
	int number_parsed_in = std::strtol(text_to_parse.c_str(), &e, dezimal_base);

	if (*e != 0){
		std::stringstream info;
		info << "StrToInt: ";
		info << "Can not parse '" << text_to_parse << "' to integer.";
		throw TracerException(info.str());
	}

	return number_parsed_in;
}