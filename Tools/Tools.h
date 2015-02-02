//=================================
// include guard
#ifndef __TOOLS_H_INCLUDED__
#define __TOOLS_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "math.h"	  
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <fstream>
#include "TracerException.h"

double Deg2Rad(double angle_in_Deg);
double Rad2Deg(double angle_in_Rad);

namespace UserInteraction {
	void print_welcome_screen();	
	std::string input(const std::string request);
	void ClearScreen();
	std::string parse_config_file_path(int number_of_arguments, char* arguments[]);
	bool is_Escape_key(const int user_input_key);
}
#endif // __TOOLS_H_INCLUDED__