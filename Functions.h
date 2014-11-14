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
void CutOutFirstToken( 
	std::string* TextToCutOutFirstToken, 
	std::string* FirstToken,
	const std::string delimiter 
);
void ClearScreen();

std::string multi( std::string text_to_repeat, unsigned int times );

bool is_ending(std::string text, std::string ending);

#endif // __FUNCTIONS_H_INCLUDED__