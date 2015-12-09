//=================================
// include guard
#ifndef __SKYDOMEMonochrom_H_INCLUDED__
#define __SKYDOMEMonochrom_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Cameras/SkyDome/Dome.h"

namespace SkyDome {

	class Monochrom : public Dome{
	protected:

		Color dome_color;
	public:
		Monochrom();
		Monochrom(const Color dome_color);
		Color get_color_for_direction(const Vector3D dir)const;
		std::string get_print()const;
	};
} // SkyDome
#endif // __SKYDOMEMonochrom_H_INCLUDED__ 
