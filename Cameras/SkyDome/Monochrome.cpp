#include "Cameras/SkyDome/Monochrome.h"
#include <sstream>

using namespace SkyDome;

Monochrom::Monochrom(const Color _dome_color) {
	dome_color = _dome_color;
}

Monochrom::Monochrom() :
	dome_color(Color::sky_blue){}

Color Monochrom::get_color_for_direction(const Vector3D dir)const {
	return Color::sky_blue;
}

std::string Monochrom::get_print()const {
	std::stringstream out;
	out << "SkyDome Monochrome " << dome_color;
	return out.str();
}