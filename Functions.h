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
#endif // __FUNCTIONS_H_INCLUDED__



