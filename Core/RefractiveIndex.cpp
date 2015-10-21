#include "RefractiveIndex.h"
#include "Core/Function/ConstantFunction.h"
//------------------------------------------------------------------------------
RefractiveIndex::RefractiveIndex(const double n) {
	refraction_vs_wavelength = new Function::Constant(
		n, Function::Limits(0.0, 900.0)
	);
}
//------------------------------------------------------------------------------
RefractiveIndex::RefractiveIndex(const Function::Func1D* n_vs_lambda) {
	refraction_vs_wavelength = n_vs_lambda;
}
//------------------------------------------------------------------------------
double RefractiveIndex::get_index()const {
	return (*refraction_vs_wavelength)(533.0);
}
//------------------------------------------------------------------------------
double RefractiveIndex::get_index(double wavelength)const {
	return (*refraction_vs_wavelength)(wavelength);
}
//------------------------------------------------------------------------------
std::string RefractiveIndex::get_print()const {
	
	std::stringstream out;
	out << "mean index: " << (*refraction_vs_wavelength)(533.0);
	return out.str();	
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const RefractiveIndex& refraci_ndex){
    os << refraci_ndex.get_print();
    return os;	
}
