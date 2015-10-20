#include "ReflectionProperties.h"
#include "Core/Function/ConstantFunction.h"
//------------------------------------------------------------------------------
ReflectionProperties::ReflectionProperties(const Function::Func1D* _reflectivity_vs_wavelength):
	reflectivity_vs_wavelength(_reflectivity_vs_wavelength) {}
//------------------------------------------------------------------------------
ReflectionProperties::ReflectionProperties(const double refl_coef) {
	assert_in_range_0_to_1(refl_coef);

	reflectivity_vs_wavelength = 
		new Function::Constant(refl_coef, Function::Limits(0.0, 900.0));
}
//------------------------------------------------------------------------------
void ReflectionProperties::assert_in_range_0_to_1(const double refl)const {

	if( refl < 0.0 || refl > 1.0) {
		std::stringstream info;
		info << "ReflectionProperties::" << __func__ << "\n";
		info << "Expected 0.0 <= reflection_coeficient <= 1.0, but actual: ";
		info << "reflection_coeficient = " << refl << "\n";
		throw TracerException(info.str());
	}	
}
//------------------------------------------------------------------------------
double ReflectionProperties::operator()()const {
	return (*reflectivity_vs_wavelength)(533.0);	
}
//------------------------------------------------------------------------------
double ReflectionProperties::operator()(const double wavelength)const {
	return (*reflectivity_vs_wavelength)(wavelength);
}
//------------------------------------------------------------------------------
std::string ReflectionProperties::get_print()const {
	std::stringstream out;
	out << "mean " << (*reflectivity_vs_wavelength)(533.0);
	return out.str();
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const ReflectionProperties& refl) {
	os << refl.get_print();
	return os;
}