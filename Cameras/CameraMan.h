//=================================
// include guard
#ifndef __CAMERAMAN_H_INCLUDE__
#define __CAMERAMAN_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CameraDevice.h"

//=================================
class CameraMan {
protected:

	bool verbosity = false;
public:

	void set_verbosity(const bool verbosity);
};
#endif // __CAMERAMAN_H_INCLUDE__