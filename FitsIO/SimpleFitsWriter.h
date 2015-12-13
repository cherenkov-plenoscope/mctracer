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
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "XyPlaneRayIntersectionEquation.h"
#include "CylinderPrismZ.h"
//=================================

namespace Fits {

	namespace Header {

		std::string get_header(std::string header);

		static const uint line_width = 80;
		static const uint block_length = 2880;
		static const std::string end_key = "END";

		namespace CommentRecord {
			std::string get_comment_record(std::string comment);

			static const std::string comment_key = "COMMENT   ";
			static const uint payload_line_width = line_width - comment_key.length();
		}		

		namespace KeyWordRecord {

			std::string key_value_and_comment(
				std::string key,
				std::string value,
				std::string comment
			);

			std::string keyword_fits_style(std::string keyword);
			std::string value_fits_style(std::string value);
			std::string comment_fits_style(std::string comment);

			static const uint max_keyword_lenght = 8;
			static const uint max_value_lenght = 20;
			static const uint max_keyword_comment_lenght = 47;
			
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

	class KWR {
	public:
		std::string key;
		std::string value;
		std::string comment;

		std::string row() {
			return Header::KeyWordRecord::key_value_and_comment(
				key, value, comment
			);
		}

		void from(const std::string row) {

			if(row.length() > 80)
				throw TracerException("FITS key word record is too long.");

			key = row.substr(0,8);

			std::string sign = row.substr(8,2);
			if(!StringTools::is_equal(sign, "= "))
				throw TracerException("FITS key word record, cant find equal sign '='.");

			value = row.substr(10,20);

			std::string comm = row.substr(30,2);
			if(!StringTools::is_equal(sign, "/ "))
				throw TracerException("FITS key word record, cant find comment sign '/'.");

			comment = row.substr(32,47);
		}
	};

	class CR {
	public:
		std::string comment;
		std::string row() {
			return Header::CommentRecord::get_comment_record(comment);
		}

		void add_from(const std::string row) {

			comment += row.substr(
				Header::CommentRecord::comment_key.length(),
				Header::CommentRecord::payload_line_width
			);
		}
	};

	class H {
	public:
		std::vector<KWR> kwrs;
		std::vector<CR> crs;

		std::string header() {

			std::stringstream head;
			for(KWR kwr: kwrs)
				head << kwr.row();

			for(CR cr: crs)
				head << cr.row();

			return Header::get_header(head.str());
		}

		void from();
	};
}
#endif // __FitsIO_SimpleWriter_H_INCLUDED__
