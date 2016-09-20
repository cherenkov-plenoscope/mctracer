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
namespace CameraMan {

class Verbosity {
protected:

	bool verbose = false;
public:

	void set_verbosity(const bool verbosity);
};
}//CameraMan
#endif // __CameraManVerbosity_H_INCLUDE__