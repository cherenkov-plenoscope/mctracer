#include "Photon.h"
//======================================================================
Photon::Photon(double baseX,double baseY,double baseZ,
double dirX,double dirY,double dirZ,
double new_wavelength){
	wavelength = new_wavelength;
	base.set(baseX,baseY,baseZ);
	dir.set(dirX,dirY,dirZ);
}
//======================================================================
Photon::Photon(Ray prototype_ray_for_photon,double new_wavelength){
	wavelength = new_wavelength;
	base = prototype_ray_for_photon.get_support();
	dir  = prototype_ray_for_photon.get_direction();
}
//======================================================================
std::string Photon::get_csv_line()const{
	uint decimal_precision = 6;
	return get_csv_line(decimal_precision);
}
//======================================================================
std::string Photon::get_csv_line(uint decimal_precision)const{

	std::stringstream out; 
	out.precision(decimal_precision);
	out << std::scientific;
	
	out << base.get_csv(decimal_precision) << ",";
	out <<  dir.get_csv(decimal_precision) << ",";
	out << wavelength;
	return out.str();
}
//======================================================================
std::string Photon::get_string()const{
	std::stringstream out; 
	out << Ray::get_string() << ", wavelength: ";
	out << wavelength*1e9 << "[nm]";
	return out.str();
}
//======================================================================
void Photon::disp()const{
	std::cout << "Photon -> " << get_string() << "\n";
}
//======================================================================
// friends of osstream
//======================================================================
std::ostream& operator<<(std::ostream& os, 
const Photon& photon_to_be_displayed){
    os << photon_to_be_displayed.get_string();
    return os;
}
