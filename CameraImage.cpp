#include "CameraImage.h"
//==================================================================
CameraImage::CameraImage(){
	ImageAlreadyAllocated=false;
}
//==================================================================
CameraImage::~CameraImage(){
	delete Image;
}
//==================================================================
void CameraImage::allocate_memory_for_image(
uint ImageResolutionInWidth,
uint ImageResolutionInHeight){
	if(ImageAlreadyAllocated){
		// memory is already allocated
	}else{
		//~ std::cout<<"CameraImage -> allocate image memory for ";
		//~ std::cout<<ImageResolutionInWidth<<"x";
		//~ std::cout<<ImageResolutionInHeight<<" pixels."<<std::endl;
		
		Image =new cv::Mat (
		ImageResolutionInWidth,
		ImageResolutionInHeight, 
		CV_8UC3);
		
		ImageAlreadyAllocated=true;
	}
}
//==================================================================
cv::Mat CameraImage::get_image()const{
	return *Image;
}
//==================================================================
void CameraImage::save_image(std::string str_image_name)const{
	// modify filename
	str_image_name += ".png";
	
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(1);			
	
	try {
		cv::imwrite(str_image_name, *Image, compression_params);
	}
	catch (std::runtime_error& ex) {
		fprintf(stderr,
		"Exception converting image to PNG format: %s\n",
		ex.what());
	}
}
//==================================================================
