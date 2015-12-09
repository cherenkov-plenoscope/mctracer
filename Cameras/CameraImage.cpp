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
CameraImage::CameraImage(const std::string filename_of_image){
	Image = new cv::Mat();
	load(filename_of_image);
}
//------------------------------------------------------------------------------
CameraImage::~CameraImage(){
	delete Image;
}
//------------------------------------------------------------------------------
void CameraImage::save(std::string filename_of_image)const{
	
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
void CameraImage::load(const std::string filename) {

	try{

		*Image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
	}catch (std::runtime_error& ex) {

		std::stringstream info;
		info << "CameraImage::" << __func__ << "()\n";
		info << "mctracer trys to load an OpenCV image called: ";
		info << filename << "\n";
		info << ex.what() << "\n",
		throw TracerException(info.str());
	}

	if(!Image->data) {

		std::stringstream info;
		info << "CameraImage::" << __func__ << "()\n";
		info << "mctracer failed to load an OpenCV image called: ";
		info << filename << "\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
uint CameraImage::get_number_of_pixels()const {
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
	const uint row, const uint col, const Color &color){
	cv::Vec3b intensity;

	intensity.val[0] = color.get_blue();
	intensity.val[1] = color.get_green();
	intensity.val[2] = color.get_red();

	Image->at<cv::Vec3b>(row,col) = intensity;
}
//------------------------------------------------------------------------------
Color CameraImage::get_pixel_row_col(const uint row, const uint col)const{
	cv::Vec3b intensity = Image->at<cv::Vec3b>(row,col);
	return Color(intensity.val[0], intensity.val[1], intensity.val[2]);
}
//------------------------------------------------------------------------------
double CameraImage::get_width_to_height_ratio()const {
	return double(get_number_of_cols())/double(get_number_of_rows());
}
//------------------------------------------------------------------------------
void CameraImage::merge_left_and_right_image_to_anaglyph_3DStereo(
	CameraImage* left_image, CameraImage* right_image
) {
	left_image->convert_to_grayscale();
	right_image->convert_to_grayscale();

	std::vector<cv::Mat> single_channels_left(3);
	cv::split(*left_image->Image, single_channels_left);
	
	std::vector<cv::Mat> single_channels_right(3);
	cv::split(*right_image->Image, single_channels_right);
	
	std::vector<cv::Mat> anaglyph_image_channels;		
	anaglyph_image_channels.push_back(single_channels_right.at(0)); //Blue
	anaglyph_image_channels.push_back(single_channels_right.at(1)); //Green
	anaglyph_image_channels.push_back(single_channels_left.at(2) ); //Red
	
	cv::merge(anaglyph_image_channels, *Image);
}
//------------------------------------------------------------------------------
void CameraImage::convert_to_grayscale() {
	cv::cvtColor(*Image, *Image, CV_RGB2GRAY);
	cv::cvtColor(*Image, *Image, CV_GRAY2RGB);
}