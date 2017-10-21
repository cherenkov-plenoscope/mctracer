//=================================
// include guard
#ifndef __PHOTON_H_INCLUDED__
#define __PHOTON_H_INCLUDED__

//=================================
// forward declared dependencies
namespace PhotonAndFrame {class Propagator;}
//=================================
// included dependencies
#include "Core/RayForPropagation.h"
#include "FresnelRefractionAndReflection.h"

//=================================
class Photon :public RayForPropagation{
	friend class PhotonAndFrame::Propagator;
protected:

	double wavelength;
public:

	Photon(
		const Vec3 support,
		const Vec3 direction,
		const double wavelength
	);
	double get_wavelength()const;
	std::string str()const;
	double get_time_of_flight()const;
	class BadWaveLength : public TracerException {
		using TracerException::TracerException;
	};
private:

	double get_time_to_pass_distance_in_refractive_index(
		const double distance_in_medium,
		const double refractive_index_in_medium
	)const;
	void assert_wavelength_is_positive()const;
};
#endif // __PHOTON_H_INCLUDED__ 
