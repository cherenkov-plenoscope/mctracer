//=================================
// include guard
#ifndef __FitsIO_SimpleWriter_H_INCLUDED__
#define __FitsIO_SimpleWriter_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "TracerException.h"
#include "Tools/StringTools.h"
//=================================

namespace Fits {

	namespace Header {

		static const uint line_width = 80;
		static const uint block_length = 2880;
		static const std::string end_key = "END";

		std::string header_fits_style(std::string header);

		namespace CommentRecord {

			static const std::string comment_key = "COMMENT   ";
			static const uint payload_line_width = line_width - comment_key.length();

			std::string get_comment_record(std::string comment);
		}		

		namespace KeyWordRecord {

			static const std::string equal_sign = "= ";
			static const std::string comment_sign = " / ";
			static const uint max_keyword_lenght = 8;
			static const uint max_value_lenght = 20;
			static const uint max_keyword_comment_lenght = 47;

			std::string key_value_and_comment(
				std::string key,
				std::string value,
				std::string comment
			);
			std::string keyword_fits_style(std::string keyword);
			std::string value_fits_style(std::string value);
			std::string comment_fits_style(std::string comment);
			
			class KeyWordTooLarge : public TracerException {
				using TracerException::TracerException;
			};

			class ValueTooLarge : public TracerException {
				using TracerException::TracerException;
			};
			
			class CommentTooLarge : public TracerException {
				using TracerException::TracerException;
			};			
		}
	}
}
#endif // __FitsIO_SimpleWriter_H_INCLUDED__
