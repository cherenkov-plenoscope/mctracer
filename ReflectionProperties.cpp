#include "ReflectionProperties.h"
//------------------------------------------------------------------------------
void ReflectionProperties::SetReflectionCoefficient(
	const double new_reflection_coefficient){

	if( new_reflection_coefficient >= 0.0 && 
		new_reflection_coefficient <= 1.0
	){
		simple_reflection_coefficient = new_reflection_coefficient;
		if(new_reflection_coefficient==0.0)
		{
			reflection_flag = false;
		}else{
			reflection_flag = true;
		}
	}else{
		throw BadValue("out of range [0,1]","reflection coefficient");
	}
}
//------------------------------------------------------------------------------
void ReflectionProperties::SetReflectionCoefficient(const std::string path2xml){
	// usethe Function1D set to parse in the xml file
	reflection_function.set(path2xml);

	// check for range of reflection coefficients
	for(
		std::vector<std::pair<double,double>>::const_iterator it = 
		reflection_function.begin();
		it < reflection_function.end();
		it++
	){
		if( it->second > 1.0 || it->second < 0.0 ){
			std::stringstream info;
			info << "reflection coefficient in file " << path2xml << "\n";			
			throw BadValue("out of range [0,1]",info.str());
		}
		// if only one value of the reflection function is above zero, the 
		// reflection flag is set to true 
		if( it->second >0.0 ){ reflection_flag = true;}
	}

	// overwrite the simple reflection coefficient
	simple_reflection_coefficient = reflection_function.mean_value();

	reflection_function_was_set = true;
}
//------------------------------------------------------------------------------
double ReflectionProperties::ReflectionCoefficient() const{

	return simple_reflection_coefficient;
}
//------------------------------------------------------------------------------
double ReflectionProperties::ReflectionCoefficient(double wavelength) const{

	if(reflection_function_was_set){
		return reflection_function.at(wavelength);
	}else{
		return simple_reflection_coefficient;
	}
}	
//------------------------------------------------------------------------------
std::string ReflectionProperties::get_string() const{

	std::stringstream out; out.str("");
	out<<"reflection flag: ";
	if(reflection_flag)
	{
		out<<"true"<<std::endl; 
		out<<"reflection coefficient: ";
		out<<simple_reflection_coefficient<<std::endl;
	}else{
		out<<"false"<<std::endl;
	}
	return out.str();
}
//------------------------------------------------------------------------------
void ReflectionProperties::disp() const{

	std::cout<<get_string();
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const ReflectionProperties& refl){

		os << "refl. coeff.=";
		os << refl.ReflectionCoefficient();
	return os;
}
