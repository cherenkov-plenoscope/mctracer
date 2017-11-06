#include "StringTools.h"
#include <algorithm>
#include <exception>
#include <sstream>
using std::string;
using std::stringstream;
using std::vector;
//------------------------------------------------------------------------------
namespace StringTools {

	bool is_equal(const string text_A,const string text_B) {
		return text_A.compare(text_B)==0 && text_A.length()==text_B.length();
	}
	//--------------------------------------------------------------------------
	string cut_leading_token_infront_of_delimiter( 
		string &text_of_tokens, 
		const char delimiter 
	) {
		// find the first delimiter
		std::size_t pos = text_of_tokens.find( delimiter );

		// Copy the first token from the text
		string first_token = text_of_tokens.substr( 0, pos );

		// now erase the first token in the text
		if( pos == string::npos ){
			// there was no delimiter found, so the whole text has to be erased
			text_of_tokens = ""; 
		}else{
			// only the characters up to the delimiter have to be erased in the text
			text_of_tokens.erase( 0, pos + 1 ); 
		}
		return first_token;
	}
	//--------------------------------------------------------------------------
	string repeat_multiple_times(
		const string text_to_repeat, 
		const unsigned int times 
	) {
		stringstream multiple_text;

		for(unsigned int i=0; i<times; i++)
			multiple_text << text_to_repeat;

		return multiple_text.str();
	}
	//--------------------------------------------------------------------------
	bool is_ending(string text, string ending) {
		std::size_t length_of_text = text.length();
		std::size_t found_first = text.find(ending);
		std::size_t found_last = text.find_last_of(ending);

		if (found_first != string::npos)
			return ((found_last+1) == length_of_text);
		else
			return false;
	}
	//--------------------------------------------------------------------------
	bool string_contains_char(const string &text, const char ch) {
		return text.find(ch) != string::npos;
	}
	//--------------------------------------------------------------------------
	string place_first_infront_of_each_new_line_of_second(
		const string front,
		string text
	) {
		stringstream out;
		while(!text.empty()) {
			out << front << cut_leading_token_infront_of_delimiter(text, '\n');
			out << '\n';
		}
		return out.str();
	}
	//--------------------------------------------------------------------------
	vector<string> tokenize_text_using_either_one_of_delimiters(
		const string &text, 
		const string delimiters
	) {
		vector<string> tokens;

	    std::size_t prev = 0;
	    std::size_t pos;
	    while((pos = text.find_first_of(delimiters, prev)) != std::string::npos) {
	        if (pos > prev)
	            tokens.push_back(text.substr(prev, pos-prev));
	        prev = pos+1;
	    }
	    if (prev < text.length())
	        tokens.push_back(text.substr(prev, std::string::npos));

		return tokens;
	}
	//--------------------------------------------------------------------------
	string strip_whitespaces(string text) {
		while(std::isspace(*text.begin()))
		    text.erase(text.begin());

		while(std::isspace(*text.rbegin()))
		    text.erase(text.length()-1);

		return text;
	}
	//--------------------------------------------------------------------------
	string fill_up_text_with_whitespaces_until_column(
		const string &text, const unsigned int column
	) {
		const unsigned int spaces_to_be_filled = column - text.length() % column;

		string spaces(spaces_to_be_filled, ' ');

		string aligned_text; 
		aligned_text += text;
		aligned_text += spaces;

		return aligned_text;
	}
	//--------------------------------------------------------------------------
	double to_double(string text_to_parse) {

		if(text_to_parse.compare("") == 0){
			stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_double: String is empty.";
			throw std::invalid_argument(info.str());
		}
		
		char *e;
		double number_parsed_in = std::strtod(text_to_parse.c_str(), &e);

		if (*e != 0){
			stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_double: ";
			info << "Can not parse '" << text_to_parse << "' to double.";
			throw std::invalid_argument(info.str());
		}

		return number_parsed_in;
	}
	//--------------------------------------------------------------------------
	bool to_bool(string text_to_parse) {

		if(text_to_parse.compare("") == 0){
			stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_bool: String is empty.";
			throw std::invalid_argument(info.str());
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
			stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_bool: Can not parse: ";
			info << "'" << text_to_parse << " to bool";
			throw std::invalid_argument(info.str());
		}
	}
	//--------------------------------------------------------------------------
	int to_int(string text_to_parse) {

		if(text_to_parse.compare("") == 0){
			stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_int: String is empty.";
			throw std::invalid_argument(info.str());
		}
		
		const int dezimal_base = 10;
		char *e;

		int number_parsed_in = std::strtol(
			text_to_parse.c_str(), 
			&e, dezimal_base
		);

		if (*e != 0){
			stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "StringTools::to_int: ";
			info << "Can not parse '" << text_to_parse << "' to int.";
			throw std::invalid_argument(info.str());
		}

		return number_parsed_in;
	}
	//--------------------------------------------------------------------------
	Tuple3 to_Tuple3(const string original_text) {

		Tuple3 t3;

		stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected Tuple3 '[float,float,float]', but actual it is ";
		info << "'" << original_text << "'.\n";

		
		string text = original_text;
		std::size_t pos = text.find("[");

		if(pos != string::npos)
			text = text.substr(pos+1);	
		else
			throw std::invalid_argument(info.str());
			
		pos = text.find(",");

		try{
			if(pos != string::npos)
				t3.x = to_double(strip_whitespaces(text.substr(0,pos)));	
			else
				throw std::invalid_argument(info.str());

			text = text.substr(pos+1);
			pos = text.find(",");

			if(pos != string::npos)
				t3.y = to_double(strip_whitespaces(text.substr(0,pos)));
			else
				throw std::invalid_argument(info.str());
			
			text = text.substr(pos+1);
			pos = text.find("]");

			if(pos != string::npos)
				t3.z = to_double(strip_whitespaces(text.substr(0,pos)));
			else
				throw std::invalid_argument(info.str());
		}catch(std::exception &error) {

	        info << "\n" << error.what() << "\n";
            throw std::invalid_argument(info.str());		
		}catch(...) {

            throw std::invalid_argument(info.str());           
        }

		return t3; 
	}
}