#include "FitsIO/SimpleFitsWriter.h"
#include <algorithm>
#include <string>

namespace Fits {
namespace Header {

std::string get_header(std::string header) {
	
	std::string out;
	out += header;
	out += end_key;

	uint blocks = out.length()/block_length + 1;
	uint fill = blocks*block_length - out.length();
	out += StringTools::repeat_multiple_times(" ", fill);;
	return out;	
}

namespace KeyWordRecord {

	std::string key_value_and_comment(
		std::string keyword,
		std::string value,
		std::string comment
	) {
		keyword = keyword_fits_style(keyword);
		std::string equal_sign = "= ";

		value = value_fits_style(value);
		std::string comment_sign = " / ";

		comment = comment_fits_style(comment);

		return keyword += equal_sign += value += comment_sign += comment;
	}

	std::string keyword_fits_style(std::string keyword) {

		// should be ASCII 48-57 and 65 to 90 -> 0 to 9 decimal and upper case A-Z
		std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::toupper);

		if(keyword.length() > max_keyword_lenght) {
		std::stringstream info;
			info << "Fits::KeyWordRecord::KeyWordTooLarge\n";
			info << "The keyword " << keyword << " is too large, max lenght is "; 
			info << std::to_string(max_keyword_lenght) << " chars.";			
			throw KeyWordTooLarge(info.str());
		}

		return keyword += StringTools::repeat_multiple_times(
			" ", max_keyword_lenght - keyword.length()
		);
	}

	std::string value_fits_style(std::string value) {

		if(value.length() > max_value_lenght) {
			std::stringstream info;
			info << "Fits::KeyWordRecord::ValueTooLarge\n";
			info << "The Value " << value << " is too large, max lenght is "; 
			info << std::to_string(max_value_lenght) << " chars.";
			throw ValueTooLarge(info.str());
		}
		
		std::string value_str = StringTools::repeat_multiple_times(
			" ", max_value_lenght - value.length()
		);

		return value_str += value;
	}

	std::string comment_fits_style(std::string comment) {

		if(comment.length() > max_keyword_comment_lenght) {
			std::stringstream info;
			info << "Fits::KeyWordRecord::CommentTooLarge\n";
			info << "The Comment " << comment << " is too large, max lenght is "; 
			info << std::to_string(max_keyword_comment_lenght) << " chars.";
			throw CommentTooLarge(info.str());
		}
			
		return comment += StringTools::repeat_multiple_times(
			" ", max_keyword_comment_lenght - comment.length()
		);
	}
}//   


namespace CommentRecord {

	std::string get_comment_record(std::string comment) {
		
		uint line_breaks = comment.length()/payload_line_width;
		uint lines = line_breaks+1;
		uint fill = lines*payload_line_width - comment.length();

		std::string out;
		for(uint line=0; line<lines; line++) {
			out += comment_key;
			out += comment.substr(
				line*payload_line_width,
				payload_line_width
			);
		}
		out += StringTools::repeat_multiple_times(" ", fill);;

		return out;
	}
}
}// HEADER
}// FITS  