//=================================
// include guard
#ifndef __SKYDOME_H_INCLUDED__
#define __SKYDOME_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <string>
#include "CameraImage.h"
#include "Core/Vector3D.h"

//------------------------------------------------------------------------------
class SkyDome {

	std::string filename;
	const CameraImage sky;
	uint central_row;
	uint central_col;
	uint zenith_to_horizon_radius;
	const double two_over_pi = 2.0/M_PI; 
public:

	SkyDome(const std::string _filename);
	Color get_color_for_direction(const Vector3D dir)const;
	std::string get_print()const;
private:

};
#endif // __SKYDOME_H_INCLUDED__ 
