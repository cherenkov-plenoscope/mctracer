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
#include "Core/Vec3.h"
//=================================
class PrismZ {
public:
	virtual bool is_inside(const Vec3* vec)const = 0;
	virtual double get_area()const {return 0.0;};
};
#endif // __PRISMZ_H_INCLUDE__
