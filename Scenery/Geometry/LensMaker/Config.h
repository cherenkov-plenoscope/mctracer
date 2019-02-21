//=================================
// include guard
#ifndef __LensMakerConfig_H_INCLUDED__
#define __LensMakerConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies

//=================================

namespace relleums {
namespace LensMaker {

struct Config {
	double focal_length;
	double aperture_radius;
	double refractive_index;
	Config();
};

}  // namespace LensMaker
}  // namespace relleums

#endif // __LensMakerConfig_H_INCLUDED__