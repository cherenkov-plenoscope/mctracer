#include "ReflectionProperties.h"
//======================================================================
ReflectionProperties::ReflectionProperties(){
	// set default values
	reflection_flag = false;
	reflection_coefficient = 0.0;
}
//======================================================================
void ReflectionProperties::SetReflectionCoefficient
(const double new_reflection_coefficient){
	if(new_reflection_coefficient >= 0.0 && 
	new_reflection_coefficient <= 1.0)
	{
		reflection_coefficient = new_reflection_coefficient;
		if(new_reflection_coefficient==0.0)
		{
			reflection_flag = false;
		}else{
			reflection_flag = true;
		}
	}else{
		// reflection_flag remains false;
		reflection_coefficient = 0.0;
		std::stringstream out; out.str("");
		out<<"Setting reflection coefficient failed!"<<std::endl;
		out<<"New reflection coefficient is out of bounds:"<<std::endl;
		out<<"0.0 < "<<new_reflection_coefficient;
		out<<" < 1.0 does not hold true.";
		out<<std::endl;
		std::cout<<out.str();
	}
}
//======================================================================
double ReflectionProperties::ReflectionCoefficient() const{
	return reflection_coefficient;
}
double ReflectionProperties::ReflectionCoefficient(double wavelength) const{
	return reflection_coefficient;
}	
//======================================================================
bool ReflectionProperties::get_reflection_flag() const{
	return reflection_flag;
}
//======================================================================
std::string ReflectionProperties::get_string() const{
	std::stringstream out; out.str("");
	out<<"reflection flag: ";
	if(reflection_flag)
	{
		out<<"true"<<std::endl; 
		out<<"reflection coefficient: ";
		out<<reflection_coefficient<<std::endl;
	}else{
		out<<"false"<<std::endl;
	}
	return out.str();
}
//======================================================================
void ReflectionProperties::disp() const{ 
	std::cout<<get_string();
}
//======================================================================
// friends of osstream
//======================================================================
std::ostream& operator<<(std::ostream& os, const ReflectionProperties& refl){
    
    if(refl.get_reflection_flag()) {
		os << "refl. coeff.=";
		os << refl.ReflectionCoefficient();
	}else{
		os << "FALSE";
	}
	return os;
}
