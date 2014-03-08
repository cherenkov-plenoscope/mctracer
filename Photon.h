//=================================
// include guard
#ifndef __PHOTON_H_INCLUDED__
#define __PHOTON_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Ray.h"
//======================================================================
class Photon :public Ray{
protected:

	double wavelength;
	//polarisation?
	
//=================================================
public:
//=================================
Photon(
double baseX,double baseY,double baseZ,
double dirX, double dirY, double dirZ,
double new_wavelength);
//=================================
Photon(Ray prototype_ray_for_photon,double new_wavelength);
//=================================
void disp()const;
std::string get_string()const;
std::string get_csv_line()const;
//=================================
std::string get_csv_line(uint decimal_precision)const;
//=================================
friend std::ostream& operator<<(std::ostream& os, 
const Photon& photon_to_be_displayed);
};
#endif // __PHOTON_H_INCLUDED__ 
