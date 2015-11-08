//=================================
// include guard
#ifndef __SKYDOME_H_INCLUDED__
#define __SKYDOME_H_INCLUDED__

//=================================
// forward declared dependencies
class Color;
//=================================
// included dependencies
#include <string>
#include "CameraImage.h"
#include "Core/Vector3D.h"
#include "Core/Printable.h"

namespace SkyDome {

	class Dome : public Printable{
	public:

		virtual Color get_color_for_direction(const Vector3D dir)const = 0;
		//virtual ~Dome();
		
	protected:

		double get_zenith_distance_of(const Vector3D dir)const;
		double get_azimuth_angle_of(const Vector3D dir)const;
	};

	class Monochrom : public Dome{
	protected:

		Color dome_color;
	public:

		Monochrom(const Color dome_color);
		//~Monochrom();
		Color get_color_for_direction(const Vector3D dir)const;
		std::string get_print()const;
	};	

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

	static const Monochrom blue_sky(Color::sky_blue);
} // SkyDome
#endif // __SKYDOME_H_INCLUDED__ 
