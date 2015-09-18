#include "AbsorptionProperties.h"
//------------------------------------------------------------------------------
AbsorptionProperties::AbsorptionProperties(const double mean_half_way_depth) {
	this->mean_half_way_depth = mean_half_way_depth;
}
//------------------------------------------------------------------------------
AbsorptionProperties::AbsorptionProperties(
	const std::string path
) {
	half_way_depth_func = new Func1D(AsciiIo::gen_table_from_file(path));
	mean_half_way_depth = half_way_depth_func->get_weighted_mean_of_value();
}
//------------------------------------------------------------------------------
double AbsorptionProperties::get_half_way_depth()const {
	return mean_half_way_depth;
}
//------------------------------------------------------------------------------
double AbsorptionProperties::get_half_way_depth(const double wavelength)const {
	if(half_way_depth_func == nullptr)
		return mean_half_way_depth;
	else
		return half_way_depth_func->interploate_value_at_argument(wavelength);
}
//------------------------------------------------------------------------------
std::string AbsorptionProperties::get_print()const {
	std::stringstream out;
	out << "half way depth: " << mean_half_way_depth << "m";
	return out.str();
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const AbsorptionProperties& absorp) {
	os << absorp.get_print();
	return os;
}