//=================================
// include guard
#ifndef __STRINGTOOLS_H_INCLUDE__
#define __STRINGTOOLS_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies 
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include "TracerException.h"

namespace StringTools {
	bool is_equal(const std::string text_A,const std::string text_B);

	std::string cut_leading_token_infront_of_delimiter( 
		std::string& text_of_tokens, const char delimiter 
	);

	std::string repeat_multiple_times( 
		std::string text_to_repeat, 
		unsigned int times 
	);

	bool is_ending(std::string text, std::string ending);

	void remove_char_from_text_if_leading(
		const char character_to_remove,
		std::string &text
	);
}

#endif // __STRINGTOOLS_H_INCLUDE__