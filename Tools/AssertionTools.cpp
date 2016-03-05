#include "Tools/AssertionTools.h"
#include <sstream>
#include "Core/TracerException.h"
#include "Tools/StringTools.h"

namespace AssertionTools {
	//--------------------------------------------------------------------------
	void value_with_name_is_greater_zero_given_context(
		const double value, 
		const std::string name, 
		const std::string context
	) {
		if(value <= 0.0) {
			std::stringstream out;
			out << "Expected " << name << " > 0.0, but actual ";
			out << name << " = " << value << ". " << context; 
			throw TracerException(out.str());
		}
	}
	//--------------------------------------------------------------------------
	void value_with_name_is_in_min_max_given_context(
		const double value, 
		const std::string name,
		const double min,
		const double max, 
		const std::string context
	) {
		if(value < min || value > max) {
			std::stringstream out;
			out << "Expected ";
			out << min << " < " << name << " < " << max << ", but actual ";
			out << name << " = " << value << ". " << context; 
			throw TracerException(out.str());
		}
	}
	//--------------------------------------------------------------------------
	void text_with_name_is_not_empty_given_context(
		const std::string text_to_check,
		const std::string name_of_text,
		const std::string context
	) {

		if(text_to_check.empty()){
			std::stringstream info;
			info << "Expected " << name_of_text << " not to be empty, ";
			info << "but actually it is empty. ";
			info << context;
			throw TracerException(info.str());
		}		
	}
	//--------------------------------------------------------------------------
	void text_with_name_has_no_whitespaces_given_context(
		const std::string text_to_check,
		const std::string name_of_text,
		const std::string context
	) {
		uint pos = 0;
		for(auto single_character : text_to_check) {
			if(isspace(single_character)) {
				std::stringstream info;
				info << "Expected " << name_of_text << " ";
				info << "to have no whitespaces, but actual the char at pos ";
				info << pos << " is a whitespace: '";
				info << text_to_check << "'. " << context;
				throw TracerException(info.str());			
			}
			pos++;
		}
	}
	//--------------------------------------------------------------------------
	void text_with_name_has_no_specific_char_given_context(
		const std::string text_to_check,
		const std::string name_of_text,
		const char specific_char,
		const std::string context
	) {
		if(StringTools::string_contains_char(text_to_check, specific_char)) {
			std::stringstream info;
			info << "Expected " << name_of_text << " to not contain any char ";
			info << "'" << specific_char << "', but actual: ";
			info << name_of_text << " = '" << text_to_check << "'. " << context;
			throw TracerException(info.str());		
		}	
	}
}