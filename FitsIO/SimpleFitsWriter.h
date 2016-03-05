//=================================
// include guard
#ifndef __FitsIO_SimpleWriter_H_INCLUDED__
#define __FitsIO_SimpleWriter_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Core/TracerException.h"
#include "Tools/StringTools.h"
//=================================
using std::string;
using std::stringstream;

namespace Fits {

	namespace Header {

		static const uint line_width = 80;
		static const uint block_length = 2880;
		static const string end_key = "END";

		string header_fits_style(string header);

		namespace CommentRecord {

			static const string comment_key = "COMMENT   ";
			static const uint payload_line_width = line_width - comment_key.length();

			string get_comment_record(string comment);
		}		

		namespace KeyWordRecord {

			static const string equal_sign = "= ";
			static const string comment_sign = " / ";
			static const uint max_keyword_lenght = 8;
			static const uint max_value_lenght = 20;
			static const uint max_keyword_comment_lenght = 47;

			string key_value_and_comment(
				string key,
				string value,
				string comment
			);
			string keyword_fits_style(string keyword);
			string value_fits_style(string value);
			string comment_fits_style(string comment);
			
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

	struct KEYWORDRECORD {
		string f;
		KEYWORDRECORD(string key, string val, string com) {
			f = Header::KeyWordRecord::key_value_and_comment(key,val,com);
		}

		string fits_style()const {
			return f;
		}
	};

	struct COMMENTRECORD {
		string f;
		COMMENTRECORD(string com) {
			f = Header::CommentRecord::get_comment_record(com);
		}

		string fits_style()const {
			return f;
		}	
	};

	struct HEADER {

		stringstream f;
		HEADER() {}

		HEADER(const HEADER& header) {
			f << header.f.str();
		}

		void operator<<(const KEYWORDRECORD kwr) {
			f << kwr.fits_style();
		}
		void operator<<(const COMMENTRECORD com) {
			f << com.fits_style();
		}
		string fits_style()const {
			return Header::header_fits_style(f.str());
		}
	};

	struct Writer {

		const string path;
		std::ofstream file;

		Writer(const string _path);
		void operator<<(const HEADER head);
		void operator<<(const string text);
		void operator<<(const float f);
		void operator<<(const double f);
		void operator<<(const int i);
		~Writer();
	};
}
#endif // __FitsIO_SimpleWriter_H_INCLUDED__
