#include "Functions.h"
//------------------------------------------------------------------------------
double Deg2Rad(double angle_in_Deg) {
	return (M_PI*angle_in_Deg)/180.0;
}
//------------------------------------------------------------------------------
double Rad2Deg(double angle_in_Rad) {
	return (angle_in_Rad/M_PI)*180.0;
}
//------------------------------------------------------------------------------
void ClearScreen() {
	std::cout << std::string( 100, '\n' );
}
//------------------------------------------------------------------------------
/*std::vector< std::string > StingOfTokens2Vector( 
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
}*/
//------------------------------------------------------------------------------
std::string cut_leading_token( 
	std::string& text_of_tokens, 
	const char delimiter 
){
	// find the first delimiter
	std::size_t pos = text_of_tokens.find( delimiter );

	// Copy the first token from the text
	std::string first_token = text_of_tokens.substr( 0, pos );

	// now erase the first token in the text
	if( pos == std::string::npos ){
		// there was no delimiter found, so the whole text has to be erased
		text_of_tokens = ""; 
	}else{
		// only the characters up to the delimiter have to be erased in the text
		text_of_tokens.erase( 0, pos + 1 ); 
	}
	return first_token;
}
//------------------------------------------------------------------------------
std::string multi( std::string text_to_repeat, unsigned int times ){

	std::stringstream multiple_text;

	for(unsigned int i=0; i<times; i++)
		multiple_text << text_to_repeat;

	return multiple_text.str();
}
//------------------------------------------------------------------------------
bool is_ending(std::string text, std::string ending){
	std::size_t length_of_text   	= text.length();
	std::size_t found_first 		= text.find(ending);
	std::size_t found_last			= text.find_last_of(ending);

	if (found_first != std::string::npos)
		return ((found_last+1) == length_of_text);
	else
		return false;
}
//------------------------------------------------------------------------------
void remove_if_leading(std::string &text, const char character_to_remove ){
	if( text.find( character_to_remove ) == 0 )
		text.erase(0,1);
}
//------------------------------------------------------------------------------
bool pedantic_str_comp(const std::string text_A,const std::string text_B){
	return (text_A.compare(text_B)==0 && text_A.length() == text_B.length() );
}
//------------------------------------------------------------------------------
namespace ToolBox {
//------------------------------------------------------------------------------
uint file_size_in_bytes(const std::string name_of_file_to_get_size_of) {
	std::ifstream file;
	file.open(name_of_file_to_get_size_of, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		std::stringstream info;
		info << "Toolbox: " << __func__ << "()\n";
		info << "Error reading file: '" << name_of_file_to_get_size_of;
		info << "'.\n";
		throw TracerException(info.str());	
	}
	return file.tellg();	
}
//------------------------------------------------------------------------------
float four_byte_str_2_float_binary_mapping(const std::string word) {

	if(word.size() != 4) {
		std::stringstream info;
		info << "Toolbox: " << __func__ << "()\n";
		info << "Expected word size: 4 bytes, but actual: ";
		info << word.size() << " bytes.\n";
		info << "word: '" << word << "'.\n";
		throw TracerException(info.str());			
	}

	char word_array[] = {word.at(0), word.at(1), word.at(2), word.at(3)};
	float f;
	memcpy(&f, &word_array, sizeof(f));
	return f;
}
//------------------------------------------------------------------------------
std::string float_2_four_byte_str_binary_mapping(const float word_in_float) {

	char word_array[4];

	memcpy(&word_array, &word_in_float, sizeof(word_array));

	std::string word;
	for(uint i=0; i<4; i++)
		word += word_array[i];

	return word;
}
//------------------------------------------------------------------------------
std::string float_vec_2_str_using_delimiter(
	const std::vector<float> vec, 
	const std::string delimiter
) {
	std::stringstream out;
	for(float element : vec) 
		out << element << delimiter;
	return out.str();
}
//------------------------------------------------------------------------------
}