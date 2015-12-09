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

	std::string beginning_of_simple_fits_header;

	namespace KeyWordRecord {

		std::string get_keyword_record_for_key_value_and_comment(
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
		static const uint fixed_length = 80;

		class KeyWordTooLarge : public TracerException {
			using TracerException::TracerException;
			std::string get_message()const;
		};

		class ValueTooLarge : public TracerException {
			using TracerException::TracerException;
			std::string get_message()const;
		};
		
		class CommentTooLarge : public TracerException {
			using TracerException::TracerException;
			std::string get_message()const;
		};			
	}
}


#endif // __FitsIO_SimpleWriter_H_INCLUDED__
