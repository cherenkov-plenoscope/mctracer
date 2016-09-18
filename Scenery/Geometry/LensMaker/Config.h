//=================================
// include guard
#ifndef __LensMakerConfig_H_INCLUDED__
#define __LensMakerConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies

//=================================

namespace LensMaker {

	struct Config {

		double focal_length;
		double aperture_radius;
		double refractive_index;
		Config();
	};
}
#endif // __LensMakerConfig_H_INCLUDED__ 