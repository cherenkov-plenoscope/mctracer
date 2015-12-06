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
#include <algorithm>
#include "TracerException.h"
#include "Core/Color.h"

double Deg2Rad(double angle_in_Deg);
double Rad2Deg(double angle_in_Rad);
bool is_odd(const int n);
bool is_even(const int n);

double StrToDouble(std::string text_to_parse);
bool StrToBool(std::string text_to_parse);
int StrToInt(std::string text_to_parse);
double get_mean_along_column(const std::vector<std::vector<double>> &table, const uint column);
std::string get_print(const std::vector<std::vector<double>> &table);

namespace UserInteraction {
	void print_welcome_screen();	
	void ClearScreen();
	bool is_Escape_key(const int user_input_key);
	std::string bg_color(const std::string &text, const Color col);
	std::string get_progress_print(const double fraction);
	std::string overwrite_last_line_with(const std::string &text);
	std::string green_font(const std::string &text);
}

namespace AssertionTools {
	void value_with_name_is_greater_zero_given_context(
		const double value, 
		const std::string name, 
		const std::string context
	);

	void value_with_name_is_in_min_max_given_context(
		const double value, 
		const std::string name,
		const double min,
		const double max, 
		const std::string context
	);

	void text_with_name_is_not_empty_given_context(
		const std::string text_to_check,
		const std::string name_of_text,
		const std::string context
	);

	void text_with_name_has_no_whitespaces_given_context(
		const std::string text_to_check,
		const std::string name_of_text,
		const std::string context
	);

	void text_with_name_has_no_specific_char_given_context(
		const std::string text_to_check,
		const std::string name_of_text,
		const char specific_char,
		const std::string context
	);
}

#endif // __TOOLS_H_INCLUDED__