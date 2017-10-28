#include "Photon.h"
#include "PhysicalConstants.h"
#include "Core/RayAndFrame.h"
#include <sstream>
//------------------------------------------------------------------------------
Photon::Photon(
	const Vec3 support,
	const Vec3 direction,
	const double _wavelength
): 	
	RayForPropagation(support, direction), 
	wavelength(_wavelength)
{
	assert_wavelength_is_positive();
}
//------------------------------------------------------------------------------
void Photon::assert_wavelength_is_positive()const {
	
	if(wavelength <= 0.0 ) {
		std::stringstream info;
		info << __FILE__ << " " << __LINE__ << "\n";
		info << "Expected photon wavelength lambda > 0.0nm, but actual ";
		info << "it is: " << wavelength*1e9 << "nm\n";
		throw std::invalid_argument(info.str());
	}
}
//------------------------------------------------------------------------------
double Photon::get_wavelength()const {
	return wavelength;
}
//------------------------------------------------------------------------------
std::string Photon::str()const {
	std::stringstream out; 
	out << RayForPropagation::str();
	out << "wavelength " << wavelength*1e9 << "nm\n";
	return out.str();
}
//------------------------------------------------------------------------------
double Photon::get_time_to_pass_distance_in_refractive_index(
	const double distance_in_medium,
	const double refractive_index_in_medium
)const {
	return refractive_index_in_medium * 
		distance_in_medium / PhysicalConstants::speed_of_light_in_vacuum;
}
//------------------------------------------------------------------------------
double Photon::get_time_of_flight()const {

	double time_of_flight = 0.0;

	uint i =0;
	for(const Intersection &intersec : intersection_history) {
		if(i!=0)
			time_of_flight += get_time_to_pass_distance_in_refractive_index(
				intersec.get_intersection_distance(),
				intersec.get_refractive_index_coming_from(wavelength)
			);
		i++;
	}
	
	return time_of_flight;
}
//------------------------------------------------------------------------------