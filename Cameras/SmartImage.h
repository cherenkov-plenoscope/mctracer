//=================================
// include guard
#ifndef __SMARTIMAGE_H_INCLUDED__
#define __SMARTIMAGE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <cv.h>
#include <highgui.h>
#include <vector>
#include <list>
#include <opencv2/opencv.hpp>
#include "TracerException.h"
#include <algorithm>

//==============================================================================
struct PixelFlag{
	bool HasBeenCalculated;
	uint row;
	uint col;
};

enum GraphicArraySpecs { 
	MCT_QUAD, 
	MCT_QVGA, 
	MCT_VGA, 
	MCT_XGA,
	MCT_WXGA,
	MCT_WQXGA
};

class SmartImage{
protected:
	// prototype image
	uint PrototypeImageWidth;
	uint PrototypeImageHight;

	// depth of quadtree structure
	uint DepthOfQuadTree;

	uint NumberOfAllPixels;

	// Pyramide Image
	std::vector< cv::Mat* > ImagePyramide;

	std::vector< std::vector < PixelFlag* > > PixelFlagImage;
	std::list< PixelFlag* > PixelFlagList;

	//cv::Mat* 				UpdateFlags;
	//=================================
	uint Width(uint layer)const;
	uint Hight(uint layer)const;
	void Set(
		const uint newPrototypeImageWidth,
		const uint newPrototypeImageHight,
		const uint newDepthOfQuadTree
	);
public:
	//=================================
	void Set( GraphicArraySpecs Specs );
	//=================================
	~SmartImage();
	//=================================
	uint Width()const{ return Width(DepthOfQuadTree); };
	uint Hight()const{ return Hight(DepthOfQuadTree); };
	//=================================
	//std::vector< >
	//=================================
	void save(std::string image_name)const;
	//=================================
	cv::Mat* PointerToImage()const;
	/*
	std::vector<std::vector<uint>> ListOfPixelsToUpdate(double fraction);
	void Update();*/
};
#endif // __SMARTIMAGE_H_INCLUDED__
