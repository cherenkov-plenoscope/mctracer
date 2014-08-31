#include "SmartImage.h"
//==============================================================================
//SmartImage::SmartImage(){
//}
//==============================================================================
void SmartImage::Set( GraphicArraySpecs Specs ){

	switch( Specs ){
  	case MCT_QUAD:  Set(1,1,7); break; // 128 x 128
  	case MCT_QVGA:  Set(20,15,4); break; // 320 x 240
  	case MCT_VGA:   Set(20,15,5); break; // 640 x 480
	case MCT_XGA:   Set( 4, 3,8); break; // 1024 x 768
	case MCT_WXGA:  Set(40,25,5); break; // 1280 x 800
	case MCT_WQXGA: Set(40,25,6); break; // 2560 x 1600
  	default: Set(20,15,4); // 320 x 240
  	}
}
//==============================================================================
void SmartImage::Set(
	const uint newPrototypeImageWidth,
	const uint newPrototypeImageHight,
	const uint newDepthOfQuadTree
){
	PrototypeImageWidth = newPrototypeImageWidth;
    PrototypeImageHight = newPrototypeImageHight;
	DepthOfQuadTree = newDepthOfQuadTree;
	// initialize pyramide image with its layers
	for(uint layer = 0; layer <= DepthOfQuadTree; layer++){
		
		cv::Mat* Image = new cv::Mat(Width(layer),Hight(layer),CV_8UC3);
		ImagePyramide.push_back( Image );
	}

	//initialize the update flag image and list
	
	for( uint col = 0; col < Width(); col++){	

		std::vector< PixelFlag* > PixelFlagColumn;

		for( uint row = 0; row < Hight(); row++){

			PixelFlag* SinglePixelFlag = new PixelFlag;

			SinglePixelFlag -> HasBeenCalculated = FALSE;
			SinglePixelFlag -> row = row;
			SinglePixelFlag -> col = col;

			PixelFlagColumn.push_back( SinglePixelFlag );

			PixelFlagList.push_back( SinglePixelFlag );
		}
		PixelFlagImage.push_back( PixelFlagColumn );
	}

	NumberOfAllPixels = Hight(DepthOfQuadTree)*Width(DepthOfQuadTree);
}
//==============================================================================
SmartImage::~SmartImage(){
	//delete Image;
}
//==============================================================================
uint SmartImage::Width(uint layer)const{ 
	// returns the width of the final image
	return PrototypeImageWidth * pow(2,layer);
}
//==============================================================================
uint SmartImage::Hight(uint layer)const{
	return PrototypeImageHight * pow(2,layer);
}
//==============================================================================
cv::Mat* SmartImage::PointerToImage()const{
	return ImagePyramide[DepthOfQuadTree];
}
//==============================================================================
void SmartImage::save(std::string image_name)const{
	// modify filename
	image_name += ".png";
	
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(1);			
	
	try{ 
		cv::imwrite(
			image_name, 
			*ImagePyramide[DepthOfQuadTree], 
			compression_params
		); 
	}catch(std::runtime_error& ex){

		fprintf(stderr,
		"Exception converting image to PNG format: %s\n",
		ex.what());
	}
}
//==============================================================================
/*
std::vector<std::vector<uint>>SmartImage::ListOfPixelsToUpdate(double fraction){

	if(fraction <= 0.0 || fraction > 1.0){
		throw BadValue(
			"Must be non zero positive and must not exceed one.",
			"fraction"
		);
	}

	double _NumOfPixToCalc = double(NumberOfAllPixels)*fraction;

	uint NumOfPixToCalc = uint(floor(_NumOfPixToCalc));

	std::vector<std::vector<uint>> hans;
	return hans;
}
//==============================================================================
void SmartImage::Update(){
	
}
*/