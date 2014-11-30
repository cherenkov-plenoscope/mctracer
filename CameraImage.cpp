#include "CameraImage.h"
//------------------------------------------------------------------------------
CameraImage::CameraImage(const uint _cols,const uint _rows):
cols(_cols),rows(_rows){
	Image =new cv::Mat (rows, cols,	CV_8UC3);
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
 	return cols; 
}
//------------------------------------------------------------------------------
uint CameraImage::get_number_of_rows()const {
 	return rows; 
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