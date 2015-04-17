#include "ReflectionProperties.h"
//------------------------------------------------------------------------------
ReflectionProperties::ReflectionProperties(const std::string path2xml) {
	SetReflectionCoefficient(path2xml);
}
//------------------------------------------------------------------------------
ReflectionProperties::ReflectionProperties(const double refl_coef) {
	SetReflectionCoefficient(refl_coef);
}
//------------------------------------------------------------------------------
void ReflectionProperties::SetReflectionCoefficient(
	const double new_reflection_coefficient
) {
	assert_in_range_0_to_1(new_reflection_coefficient);

	simple_reflection_coefficient = new_reflection_coefficient;
	reflection_flag = (new_reflection_coefficient == 0) ? false : true;
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
void ReflectionProperties::SetReflectionCoefficient(const std::string path2xml){
	// usethe Function1D set to parse in the xml file

	reset_reflection_function();

	reflection_function = new Function1D(path2xml);

	/*
	for(
		std::vector<std::pair<double,double>>::const_iterator it = 
		reflection_function->begin();
		it < reflection_function->end();
		it++
	){
		assert_in_range_0_to_1(it->second);

		// if only one value of the reflection function is above zero, the 
		// reflection flag is set to true 
		if( it->second >0.0 ){ }
	}*/
	reflection_flag = true;
	// overwrite the simple reflection coefficient
	simple_reflection_coefficient = 
		reflection_function->get_weighted_mean_of_value();

	reflection_function_was_set = true;
}
//------------------------------------------------------------------------------
double ReflectionProperties::ReflectionCoefficient()const {
	return simple_reflection_coefficient;
}
//------------------------------------------------------------------------------
double ReflectionProperties::ReflectionCoefficient(double wavelength)const {
	if(reflection_function_was_set)
		return reflection_function->at(wavelength);
	else
		return simple_reflection_coefficient;
}	
//------------------------------------------------------------------------------
std::string ReflectionProperties::get_print()const {
	std::stringstream out;
	if(reflection_flag) {

		out << "mean " << simple_reflection_coefficient;

		if(reflection_function_was_set)
			out << ", file: " << reflection_function->get_XmlName();
	}else
		out << "none";
	
	return out.str();
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const ReflectionProperties& refl) {
	os << refl.get_print();
	return os;
}
//------------------------------------------------------------------------------
bool ReflectionProperties::flag()const {
	return reflection_flag;
}
//------------------------------------------------------------------------------
void ReflectionProperties::reset_reflection_function() {
	if(reflection_function != nullptr) {
		delete reflection_function;
		reflection_function = nullptr;
	}	
}