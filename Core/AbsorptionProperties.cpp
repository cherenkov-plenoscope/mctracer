#include "AbsorptionProperties.h"
//------------------------------------------------------------------------------
AbsorptionProperties::AbsorptionProperties(const double mean_half_way_depth) {
	this->mean_half_way_depth = mean_half_way_depth;
}
//------------------------------------------------------------------------------
AbsorptionProperties::AbsorptionProperties(
	const std::string path_to_half_way_depth
) {
	reset_function();
	half_way_depth_function = new Function1D(path_to_half_way_depth);
	mean_half_way_depth = half_way_depth_function->get_weighted_mean_of_value();
}
//------------------------------------------------------------------------------
double AbsorptionProperties::get_half_way_depth()const {
	return mean_half_way_depth;
}
//------------------------------------------------------------------------------
double AbsorptionProperties::get_half_way_depth(const double wavelength)const {
	if(half_way_depth_function == nullptr)
		return mean_half_way_depth;
	else
		return half_way_depth_function->at(wavelength);
}
//------------------------------------------------------------------------------
std::string AbsorptionProperties::get_print()const {
	std::stringstream out;
	out << "half way depth: " << mean_half_way_depth << "m";
	if(half_way_depth_function != nullptr)
		out << ", file: " << half_way_depth_function->get_XmlName();
	return out.str();
}
//------------------------------------------------------------------------------
void AbsorptionProperties::reset_function() {
	if(half_way_depth_function != nullptr) {
		delete half_way_depth_function;
		half_way_depth_function = nullptr;
	}
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const AbsorptionProperties& absorp) {
	os << absorp.get_print();
	return os;
}