//=================================
// include guard
#ifndef __FUNCTIONS_H_INCLUDED__
#define __FUNCTIONS_H_INCLUDED__

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
std::vector< std::string > StingOfTokens2Vector( 
	std::string TextToBeTokenized, 
	std::string delimiter 
);

std::string cut_leading_token( 
	std::string& text_of_tokens, const char delimiter 
);
void ClearScreen();

std::string multi( std::string text_to_repeat, unsigned int times );

bool is_ending(std::string text, std::string ending);

void remove_if_leading(std::string &text, const char character_to_remove );

bool pedantic_str_comp(const std::string text_A,const std::string text_B);
namespace ToolBox {
	
	uint file_size_in_bytes(const std::string name_of_file_to_get_size_of);
	
	float str2float_4byte_bin_map(const std::string word);
	
	std::string float2str_4byte_bin_map(const float word_in_float);
	
	std::string float_vec_2_str_using_delimiter(
		const std::vector<float> vec, 
		const std::string delimiter
	);
	void print_welcome_screen();
}

namespace UserInteraction {
	
	std::string input(const std::string request);
}
#endif // __FUNCTIONS_H_INCLUDED__