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
#include <sstream>
#include <vector>

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

#endif // __FUNCTIONS_H_INCLUDED__