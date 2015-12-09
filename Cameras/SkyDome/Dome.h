//=================================
// include guard
#ifndef __SKYDOMEDOME_H_INCLUDED__
#define __SKYDOMEDOME_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Vector3D.h"
#include "Core/Printable.h"
#include "Core/Color.h"

namespace SkyDome {

	class Dome : public Printable{
	public:

		virtual Color get_color_for_direction(const Vector3D dir)const = 0;
		virtual ~Dome() {};
		
	protected:

		double get_zenith_distance_of(const Vector3D dir)const;
		double get_azimuth_angle_of(const Vector3D dir)const;
	};
} // SkyDome
#endif // __SKYDOMEDOME_H_INCLUDED__ 
