#include "FitsIO/SimpleFitsWriter.h"
#include <algorithm>
#include <string>

namespace Fits {
namespace KeyWordRecord {

	std::string get_keyword_record_for_key_value_and_comment(
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
			
		return value += StringTools::repeat_multiple_times(
			" ", max_value_lenght - value.length()
		);
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

}//  