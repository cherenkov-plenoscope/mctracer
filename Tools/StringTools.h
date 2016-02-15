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
#include "Tuple3.h"
using std::string;

namespace StringTools {
	bool is_equal(const string text_A,const string text_B);

	string cut_leading_token_infront_of_delimiter( 
		string &text_of_tokens, const char delimiter 
	);

	string repeat_multiple_times( 
		const string text_to_repeat, 
		const unsigned int times 
	);

	bool is_ending(string text, string ending);

	bool string_contains_char(const string &text, const char ch);

	string place_first_infront_of_each_new_line_of_second(
		const string front,
		string text
	);

	std::vector<string> tokenize_text_given_delimiter_char(
		const string &text, 
		const char delimiter
	);

	string strip_whitespaces(string text);

	string fill_up_text_with_whitespaces_until_column(
		const string &text, const uint column
	);

	class CanNotParseDouble : public TracerException {
    	using TracerException::TracerException;
	};

	double to_double(string text_to_parse );

	class CanNotParseBool : public TracerException {
    	using TracerException::TracerException;
	};

	bool to_bool(string text_to_parse );

	class CanNotParseInt : public TracerException {
    	using TracerException::TracerException;
	};

	int to_int(string text_to_parse );

	class CanNotParseTuple3 : public TracerException {
	    using TracerException::TracerException;
	};

	Tuple3 to_Tuple3(const string original_text);
}

#endif // __STRINGTOOLS_H_INCLUDE__