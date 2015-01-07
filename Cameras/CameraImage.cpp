#include "CameraImage.h"
//------------------------------------------------------------------------------
CameraImage::CameraImage(const uint cols,const uint rows){
	Image =new cv::Mat (rows, cols,	CV_8UC3);
}
//------------------------------------------------------------------------------
CameraImage::CameraImage(const CameraImage* image_to_copy_from){
	Image = new cv::Mat();
	*Image = image_to_copy_from->Image->clone();
}
//------------------------------------------------------------------------------
CameraImage::~CameraImage(){
	delete Image;
}
//------------------------------------------------------------------------------
cv::Mat CameraImage::get_image()const{
	return *Image;
}
//------------------------------------------------------------------------------
void CameraImage::save_image_to_file(std::string filename_of_image)const{
	
	filename_of_image += ".png";
	
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(1);			
	
	try {
		cv::imwrite(filename_of_image, *Image, compression_params);
	}
	catch (std::runtime_error& ex) {

		std::stringstream info;
		info << "CameraImage::" << __func__ << "()\n";
		info << "mctracer trys to save an OpenCV image called: ";
		info << filename_of_image<< "\n";
		info << "Exception converting image to PNG format: " << ex.what();
		info << "\n",
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
uint CameraImage::get_resolution()const {
	return get_number_of_cols() * get_number_of_rows();
}
//------------------------------------------------------------------------------
uint CameraImage::get_number_of_cols()const {
 	return Image->cols; 
}
//------------------------------------------------------------------------------
uint CameraImage::get_number_of_rows()const {
 	return Image->rows; 
}
//------------------------------------------------------------------------------
void CameraImage::set_pixel_row_col_to_color(
	const uint row, const uint col, const ColourProperties &color){
	cv::Vec3b intensity;

	intensity.val[0] = color.get_blue();
	intensity.val[1] = color.get_green();
	intensity.val[2] = color.get_red();

	Image->at<cv::Vec3b>(row,col) = intensity;
}
//------------------------------------------------------------------------------
double CameraImage::get_width_to_height_ratio()const {
	return double(get_number_of_cols())/double(get_number_of_rows());
}
//------------------------------------------------------------------------------
void CameraImage::merge_left_and_right_image_to_anaglyph_3DStereo(
	CameraImage* left_image, CameraImage* right_image
) {
	// convert both input images to grayscale
	cv::cvtColor(*left_image->Image, *left_image->Image, CV_RGB2GRAY);
	cv::cvtColor(*left_image->Image, *left_image->Image, CV_GRAY2RGB);

	cv::cvtColor(*right_image->Image, *right_image->Image, CV_RGB2GRAY);
	cv::cvtColor(*right_image->Image, *right_image->Image, CV_GRAY2RGB);

	// BGR
	std::vector<cv::Mat> BGR_left(3);
	cv::split(*left_image->Image, BGR_left);
	
	std::vector<cv::Mat> BGR_right(3);
	cv::split(*right_image->Image, BGR_right);
	
	std::vector<cv::Mat> anaglyph_image_channels;		

	// 0 -> B 
	anaglyph_image_channels.push_back(BGR_right.at(0));
	// 1 -> G 
	anaglyph_image_channels.push_back(BGR_right.at(1));
	// 2 -> R 
	anaglyph_image_channels.push_back(BGR_left.at(2) );
	
	cv::merge(anaglyph_image_channels, *Image);
}