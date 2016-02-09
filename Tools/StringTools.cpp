#include "StringTools.h"
#include <algorithm>
//------------------------------------------------------------------------------
namespace StringTools {

	bool is_equal(const std::string text_A,const std::string text_B) {
		return text_A.compare(text_B)==0 && text_A.length()==text_B.length();
	}
	//--------------------------------------------------------------------------
	std::string cut_leading_token_infront_of_delimiter( 
		std::string &text_of_tokens, 
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
		const std::string text_to_repeat, 
		const unsigned int times 
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
	bool string_contains_char(const std::string &text, const char ch) {
		return text.find(ch) != std::string::npos;
	}
	//--------------------------------------------------------------------------
	std::string place_first_infront_of_each_new_line_of_second(
		const std::string front,
		std::string text
	) {
		std::stringstream out;
		while(!text.empty()) {
			out << front << cut_leading_token_infront_of_delimiter(text, '\n');
			out << '\n';
		}
		return out.str();
	}
	//--------------------------------------------------------------------------
	std::vector<std::string> tokenize_text_given_delimiter_char(
		const std::string &text, 
		const char delimiter
	) {
		std::vector<std::string> tokens;
		std::stringstream text_stream(text);
		std::string item;

		while(std::getline(text_stream, item, delimiter))
			tokens.push_back(item);
	
		return tokens;
	}
	//--------------------------------------------------------------------------
	std::string strip_whitespaces(std::string text) {
		while(std::isspace(*text.begin()))
		    text.erase(text.begin());

		while(std::isspace(*text.rbegin()))
		    text.erase(text.length()-1);

		return text;
	}
	//--------------------------------------------------------------------------
	std::string fill_up_text_with_whitespaces_until_column(
		const std::string &text, const uint column
	) {
		const uint spaces_to_be_filled = column - text.length() % column;

		std::string spaces(spaces_to_be_filled, ' ');

		std::string aligned_text; 
		aligned_text += text;
		aligned_text += spaces;

		return aligned_text;
	}
	//--------------------------------------------------------------------------
	double to_double(std::string text_to_parse) {

		if(text_to_parse.compare("") == 0){
			std::stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_double: String is empty.";
			throw CanNotParseDouble(info.str());
		}
		
		char *e;
		double number_parsed_in = std::strtod(text_to_parse.c_str(), &e);

		if (*e != 0){
			std::stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_double: ";
			info << "Can not parse '" << text_to_parse << "' to double.";
			throw CanNotParseDouble(info.str());
		}

		return number_parsed_in;
	}
	//--------------------------------------------------------------------------
	bool to_bool(std::string text_to_parse) {

		if(text_to_parse.compare("") == 0){
			std::stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_bool: String is empty.";
			throw CanNotParseBool(info.str());
		}
		
		std::transform(
			text_to_parse.begin(), 
			text_to_parse.end(), 
			text_to_parse.begin(), 
			::tolower
		);

		if(StringTools::is_equal(text_to_parse,"true"))
			return true;
		else if(StringTools::is_equal(text_to_parse,"false"))
			return false;
		else {
			std::stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_bool: Can not parse: ";
			info << "'" << text_to_parse << " to bool";
			throw CanNotParseBool(info.str());
		}
	}
	//--------------------------------------------------------------------------
	int to_int(std::string text_to_parse) {

		if(text_to_parse.compare("") == 0){
			std::stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_int: String is empty.";
			throw CanNotParseInt(info.str());
		}
		
		const int dezimal_base = 10;
		char *e;

		int number_parsed_in = std::strtol(
			text_to_parse.c_str(), 
			&e, dezimal_base
		);

		if (*e != 0){
			std::stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_int: ";
			info << "Can not parse '" << text_to_parse << "' to int.";
			throw CanNotParseInt(info.str());
		}

		return number_parsed_in;
	}
	//--------------------------------------------------------------------------
	Tuple3 to_Tuple3(const std::string original_text) {

		Tuple3 t3;

		std::stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected Tuple3 '[float,float,float]', but actual it is ";
		info << "'" << original_text << "'.\n";

		
		std::string text = original_text;
		std::size_t pos = text.find("[");

		if(pos != std::string::npos)
			text = text.substr(pos+1);	
		else
			throw CanNotParseTuple3(info.str());
			
		pos = text.find(",");

		try{
			if(pos != std::string::npos)
				t3.x = to_double(strip_whitespaces(text.substr(0,pos)));	
			else
				throw CanNotParseTuple3(info.str());

			text = text.substr(pos+1);
			pos = text.find(",");

			if(pos != std::string::npos)
				t3.y = to_double(strip_whitespaces(text.substr(0,pos)));
			else
				throw CanNotParseTuple3(info.str());
			
			text = text.substr(pos+1);
			pos = text.find("]");

			if(pos != std::string::npos)
				t3.z = to_double(strip_whitespaces(text.substr(0,pos)));
			else
				throw CanNotParseTuple3(info.str());
		}catch(std::exception &error) {

	        info << "\n" << error.what() << "\n";
            throw CanNotParseTuple3(info.str());		
		}catch(...) {

            throw CanNotParseTuple3(info.str());           
        }

		return t3; 
	}
}