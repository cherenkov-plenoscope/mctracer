#include "Functions.h"
//------------------------------------------------------------------------------
double Deg2Rad(double angle_in_Deg){
	return (M_PI*angle_in_Deg)/180.0;
}
//------------------------------------------------------------------------------
double Rad2Deg(double angle_in_Rad){
	return (angle_in_Rad/M_PI)*180.0;
}
//------------------------------------------------------------------------------
void ClearScreen(){
	std::cout << std::string( 100, '\n' );
}
//------------------------------------------------------------------------------
std::vector< std::string > StingOfTokens2Vector( 
	std::string TextToBeTokenized, 
	std::string delimiter 
){
	//--------------------------------------------------------------------------	
	// tokenize the given string
	//--------------------------------------------------------------------------
	std::vector< std::string > VectorOfTokens;
	std::size_t pos = 0;
	std::string SingleToken;
	while( (pos = TextToBeTokenized.find( delimiter )) != std::string::npos ){

		SingleToken = TextToBeTokenized.substr( 0, pos );
		VectorOfTokens.push_back( SingleToken );
		TextToBeTokenized.erase( 0, pos+delimiter.length() ); 
	}
	// add the remaining name after last delimiter
	VectorOfTokens.push_back( TextToBeTokenized ); 

	return VectorOfTokens;
}
//------------------------------------------------------------------------------
void CutOutFirstToken( 
	std::string* TextToCutOutFirstToken, 
	std::string* FirstToken,
	const std::string delimiter 
){
	// find the first delimiter
	std::size_t pos = TextToCutOutFirstToken->find( delimiter );

	// Copy the first token from the text
	*FirstToken = TextToCutOutFirstToken->substr( 0, pos );

	// now erase the first token in the text
	if( pos == std::string::npos ){
		// there was no delimiter found, so the whole text has to be erased
		*TextToCutOutFirstToken = ""; 
	}else{
		// only the characters up to the delimiter have to be erased in the text
		TextToCutOutFirstToken->erase( 0, pos + delimiter.length() ); 
	}
}
//------------------------------------------------------------------------------
std::string multi( std::string text_to_repeat, unsigned int times ){

	std::stringstream multiple_text;

	for(unsigned int i=0; i<times; i++){

		multiple_text << text_to_repeat;
	}
	return multiple_text.str();
}
//------------------------------------------------------------------------------
bool is_ending(std::string text, std::string ending){
	std::size_t length_of_ending 	= ending.length();
	std::size_t length_of_text   	= text.length();
	std::size_t found_first 		= text.find(ending);

	if (found_first != std::string::npos){
		return (found_first+length_of_ending == length_of_text );
	}else{
		return false;
	}
};