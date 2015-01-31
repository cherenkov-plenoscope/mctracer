//=================================
// include guard
#ifndef __PRISMZ_H_INCLUDE__
#define __PRISMZ_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include <math.h>
#include "Vector3D.h"

//=================================
class PrismZ {
public:
	virtual bool is_inside(const Vector3D* vec)const = 0;
};
#endif // __PRISMZ_H_INCLUDE__
