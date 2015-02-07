#include "SurfaceProperties.h"
//------------------------------------------------------------------------------
void SurfaceProperties::set_reflection(const ReflectionProperties* reflection) {
	this->reflection = reflection;
}
//------------------------------------------------------------------------------
void SurfaceProperties::set_color(const ColourProperties* color) {
	this->color = color;
}
//------------------------------------------------------------------------------
//void SurfaceProperties::set_BRDF(const BidirectionalReflectanceDistributionFunction* brdf);
//------------------------------------------------------------------------------
bool SurfaceProperties::has_reflection()const {
	return reflection != nullptr;
}
//------------------------------------------------------------------------------
bool SurfaceProperties::has_color()const {
	return color != nullptr;
}
//------------------------------------------------------------------------------
/*bool SurfaceProperties::has_BRDF()const {
	return brdf != nullptr;
}*/
//------------------------------------------------------------------------------
const ColourProperties* SurfaceProperties::get_color()const {
	return color;
}
//------------------------------------------------------------------------------
const ReflectionProperties* SurfaceProperties::get_reflection()const {
	return reflection;
}
//------------------------------------------------------------------------------
//const BidirectionalReflectanceDistributionFunction* SurfaceProperties::get_BDRF()const;
//------------------------------------------------------------------------------
std::string SurfaceProperties::get_print()const {
	std::stringstream out;
	out << "color: ";

	if(has_color())
		out << color;
	else
		out << "-";

	out << ", reflection: ";

	if(has_reflection())
		out << reflection;
	else 
		out << "-";
	
	out << ", BDRF: ";
/*
	if(has_BRDF())
		out << bdrf;
	else*/
		out << "-";

	out << "\n";
	return out.str();
}