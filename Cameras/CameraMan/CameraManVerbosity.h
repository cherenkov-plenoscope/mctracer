//=================================
// include guard
#ifndef __CameraManVerbosity_H_INCLUDE__
#define __CameraManVerbosity_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Cameras/CameraDevice.h"
//=================================
class CameraManVerbosity {
protected:

	bool verbose = false;
public:

	void set_verbosity(const bool verbosity);
};
#endif // __CameraManVerbosity_H_INCLUDE__