//=================================
// include guard
#ifndef __SKYDOMETexture_H_INCLUDED__
#define __SKYDOMETexture_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Cameras/SkyDome/Dome.h"
#include "Cameras/CameraImage.h"

namespace SkyDome {

	class Texture : public Dome{
	protected:

		std::string filename;
		const CameraImage sky;
		uint central_row;
		uint central_col;
		uint zenith_to_horizon_radius;
		const double two_over_pi = 2.0/M_PI;
	public:

		Texture(const std::string _filename);
		Color get_color_for_direction(const Vector3D dir)const;
		std::string get_print()const;
	protected:

		Color sky_dome_color_for(const Vector3D dir)const;	
	};	
} // SkyDome
#endif // __SKYDOMETexture_H_INCLUDED__ 
