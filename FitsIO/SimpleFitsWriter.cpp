#include "FitsIO/SimpleFitsWriter.h"
#include <algorithm>
#include <string>

namespace Fits {


Writer::Writer(const string _path):path(_path) {

	file.open(path, std::ios::out | std::ios::binary);
}

void Writer::operator<<(const HEADER head) {
	file << head.fits_style(); 
}

void Writer::operator<<(const string text) {
	file << text;
}

void Writer::operator<<(const float f) {
	file.write( (char*)&f, sizeof(float));
}

void Writer::operator<<(const double f) {
	file.write( (char*)&f, sizeof(double));
}

void Writer::operator<<(const int i) {
	file.write( (char*)&i, sizeof(int));
}

Writer::~Writer() {

	file.close();
}

string KeyValCom(string key, string val, string com) {
	return Header::KeyWordRecord::key_value_and_comment(key,val,com);
}

namespace Header {

string header_fits_style(string header) {
	
	string out;
	out += header;
	out += end_key;

	uint blocks = out.length()/block_length + 1;
	uint fill = blocks*block_length - out.length();
	out += StringTools::repeat_multiple_times(" ", fill);;
	return out;	
}

namespace KeyWordRecord {

	string key_value_and_comment(
		string keyword,
		string value,
		string comment
	) {
		
		stringstream out;
		out << keyword_fits_style(keyword);
		out << equal_sign;
		out << value_fits_style(value);
		out << comment_sign;
		out << comment_fits_style(comment);
		return out.str();
	}

	string keyword_fits_style(string keyword) {

		// should be ASCII 48-57 and 65 to 90 -> 0 to 9 decimal and upper case A-Z
		std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::toupper);

		if(keyword.length() > max_keyword_lenght) {
			
			stringstream info;
			info << "Fits::KeyWordRecord::KeyWordTooLarge\n";
			info << "The keyword " << keyword << " is too large, max lenght is "; 
			info << std::to_string(max_keyword_lenght) << " chars.";			
			throw KeyWordTooLarge(info.str());
		}

		return keyword += StringTools::repeat_multiple_times(
			" ", max_keyword_lenght - keyword.length()
		);
	}

	string value_fits_style(string value) {

		if(value.length() > max_value_lenght) {
			
			stringstream info;
			info << "Fits::KeyWordRecord::ValueTooLarge\n";
			info << "The Value " << value << " is too large, max lenght is "; 
			info << std::to_string(max_value_lenght) << " chars.";
			throw ValueTooLarge(info.str());
		}
		
		string value_str = StringTools::repeat_multiple_times(
			" ", max_value_lenght - value.length()
		);

		return value_str += value;
	}

	string comment_fits_style(string comment) {

		if(comment.length() > max_keyword_comment_lenght) {
			
			stringstream info;
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

	string get_comment_record(string comment) {
		
		uint line_breaks = comment.length()/payload_line_width;
		uint lines = line_breaks+1;
		uint fill = lines*payload_line_width - comment.length();

		string out;
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