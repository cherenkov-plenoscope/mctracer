#include "CameraRay.h"
//======================================================================
std::string CameraRay::get_csv_line()const{
	uint decimal_precision = 6;
	return get_csv_line(decimal_precision);
}
//======================================================================
std::string CameraRay::get_csv_line(uint decimal_precision)const{

	std::stringstream out; 
	out.precision(decimal_precision);
	out << std::scientific;
	
	out << base.get_csv(decimal_precision) << ",";
	out << dir.get_csv(decimal_precision) << ",";
	out << int(colour.get_red()) << ",";
	out << int(colour.get_green()) << ",";
	out << int(colour.get_blue());
	return out.str();
}
//======================================================================
std::string CameraRay::get_string()const{
	std::stringstream out; 
	out << Ray::get_string() << ", colour: " << colour;
	return out.str();
}
//======================================================================
void CameraRay::disp()const{
	std::cout << "CameraRay -> " << get_string() << "\n";
}
//======================================================================
// friends of osstream
//======================================================================
std::ostream& operator<<(std::ostream& os, 
const CameraRay& camera_ray_to_be_displayed){
    os << camera_ray_to_be_displayed.get_string();
    return os;
}
