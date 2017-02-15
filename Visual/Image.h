//=================================
// include guard
#ifndef __VISUAL_IMAGE_H_INCLUDED__
#define __VISUAL_IMAGE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include "Core/TracerException.h"
#include "Core/Color.h"

//=================================
namespace Visual {

class Image{
public:
	
	cv::Mat raw_image;

	Image();
	Image(const uint cols, const uint rows);
	Image(const Image* image_to_copy_from);
	Image(const std::string filename_of_image);
	uint get_number_of_pixels()const;
	uint get_number_of_cols()const;
	uint get_number_of_rows()const;
	void save(std::string filename)const;
	void load(const std::string filename);
	void set_row_col_to_color(
		const uint row, const uint col, const Color &color
	);
	Color get_color_row_col(const uint row, const uint col)const;
	double get_width_to_height_ratio()const;
	void merge_left_and_right_image_to_anaglyph_3DStereo(
		Image* left_image, Image* right_image
	);
	void scale(const uint scale);
private:

	void convert_to_grayscale();
};

}//Visual
#endif // __VISUAL_IMAGE_H_INCLUDED__
