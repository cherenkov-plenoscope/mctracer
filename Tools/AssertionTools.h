//=================================
// include guard
#ifndef __AssertionTools_H_INCLUDED__
#define __AssertionTools_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <string>
#include <iostream>

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
#endif // __AssertionTools_H_INCLUDED__