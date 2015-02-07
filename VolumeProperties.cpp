#include "VolumeProperties.h"
//------------------------------------------------------------------------------
void VolumeProperties::set_refraction(const RefractiveIndex* refraction) {
	this->refraction = refraction;
}
//------------------------------------------------------------------------------
//void set_absorption(const AbsorptionProperties* absorption);
//------------------------------------------------------------------------------
bool VolumeProperties::has_refraction()const {
	return refraction != nullptr;
}
//------------------------------------------------------------------------------
//bool has_absorption()const;
//------------------------------------------------------------------------------
const RefractiveIndex* VolumeProperties::get_refraction()const {
	return refraction;
}
//------------------------------------------------------------------------------
//const AbsorptionProperties* get_absorption()const;
//------------------------------------------------------------------------------
std::string VolumeProperties::get_print()const {
	std::stringstream out;
	out << "refraction: ";

	if(has_refraction())
		out << refraction;
	else
		out << "-";

	out << ", absorption: ";
/*
	if(has_absorption())
		out << absorption;
	else*/
		out << "-";

	out << "\n";
	return out.str();
}
//------------------------------------------------------------------------------