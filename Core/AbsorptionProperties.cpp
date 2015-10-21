#include "AbsorptionProperties.h"
//------------------------------------------------------------------------------
AbsorptionProperties::AbsorptionProperties(const double _mean_half_way_depth) {
	mean_half_way_depth = _mean_half_way_depth;
}
//------------------------------------------------------------------------------
AbsorptionProperties::AbsorptionProperties(
	const std::string path
) {
	half_way_depth_func = new Function::LinInterpol(
		AsciiIo::gen_table_from_file(path)
	);
}
//------------------------------------------------------------------------------
double AbsorptionProperties::get_half_way_depth()const {
	return mean_half_way_depth;
}
//------------------------------------------------------------------------------
double AbsorptionProperties::get_half_way_depth(const double wavelength)const {
	return (*half_way_depth_func)(wavelength);
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