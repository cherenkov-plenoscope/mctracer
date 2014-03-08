//=================================
// include guard
#ifndef __CAMERARAY_H_INCLUDED__
#define __CAMERARAY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Ray.h"
#include "ColourProperties.h"
//======================================================================
class CameraRay :public Ray{
protected:

	ColourProperties colour;
	
//=================================================
public:
//=================================
void disp()const;
std::string get_string()const;
std::string get_csv_line()const;
std::string get_csv_line(uint decimal_precision)const;
//=================================
friend std::ostream& operator<<(std::ostream& os, 
const CameraRay& camera_ray_to_be_displayed);
};
#endif // __CAMERARAY_H_INCLUDED__ 
