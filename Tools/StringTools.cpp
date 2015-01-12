#include "StringTools.h"
//------------------------------------------------------------------------------
namespace StringTools {

	bool is_equal(const std::string text_A,const std::string text_B) {
		return text_A.compare(text_B)==0 && text_A.length()==text_B.length();
	}
	//--------------------------------------------------------------------------
	std::string cut_leading_token_infront_of_delimiter( 
		std::string& text_of_tokens, 
		const char delimiter 
	) {
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
	//--------------------------------------------------------------------------
	std::string repeat_multiple_times(
		std::string text_to_repeat, 
		unsigned int times 
	) {
		std::stringstream multiple_text;

		for(unsigned int i=0; i<times; i++)
			multiple_text << text_to_repeat;

		return multiple_text.str();
	}
	//--------------------------------------------------------------------------
	bool is_ending(std::string text, std::string ending) {
		std::size_t length_of_text = text.length();
		std::size_t found_first = text.find(ending);
		std::size_t found_last = text.find_last_of(ending);

		if (found_first != std::string::npos)
			return ((found_last+1) == length_of_text);
		else
			return false;
	}
	//--------------------------------------------------------------------------
	void remove_char_from_text_if_leading(
		const char character_to_remove,
		std::string &text
	) {
		if( text.find( character_to_remove ) == 0 )
			text.erase(0,1);
	}
}