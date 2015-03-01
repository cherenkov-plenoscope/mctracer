#include "RefractiveIndex.h"
//------------------------------------------------------------------------------
RefractiveIndex::RefractiveIndex(
	const double simple_refractive_index_independent_of_wavelength
) {
	this->simple_refractive_index_independent_of_wavelength = 
		simple_refractive_index_independent_of_wavelength;
}
//------------------------------------------------------------------------------
RefractiveIndex::RefractiveIndex(const std::string path_to_file) {

	reset_refraction_function();

	refraction_function = new Function1D(path_to_file);

	simple_refractive_index_independent_of_wavelength = 
		refraction_function->get_weighted_mean_of_value();
}
//------------------------------------------------------------------------------
double RefractiveIndex::get_index()const {
	return simple_refractive_index_independent_of_wavelength;
}
//------------------------------------------------------------------------------
double RefractiveIndex::get_index(double wavelength)const {
	return get_index();
}
//------------------------------------------------------------------------------
std::string RefractiveIndex::get_print()const {
	
	std::stringstream out;
	out << "mean index: " << simple_refractive_index_independent_of_wavelength;

	if(refraction_function != nullptr)
		out << ", file: " << refraction_function->get_XmlName();

	return out.str();	
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const RefractiveIndex& refraci_ndex){
    os << refraci_ndex.get_print();
    return os;	
}
//------------------------------------------------------------------------------
void RefractiveIndex::reset_refraction_function() {
	if(refraction_function != nullptr) {
		delete refraction_function;
		refraction_function = nullptr;
	}	
}
