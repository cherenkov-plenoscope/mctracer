//======================================================================
// include guard
#ifndef __SPLINE1D_H_INCLUDED__
#define __SPLINE1D_H_INCLUDED__

//======================================================================
// forward declared dependencies

//======================================================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
//=======================================================================

struct SplineTuple{
	double X;
	double Y;
};

class Spline1D {
	// this class stores reflection information of an object
private:	
	std::list< SplineTuple* > Spline;
public:
//----------------
	Spline1D();
//----------------
	~Spline1D(){
		for( SplineTuple* tuple : Spline )
			delete tuple;
	};
//----------------
	add( double new_X, double new_Y ){
		SplineTuple* a_new_tuple;
		a_new_tuple = new SplineTuple;
		a_new_tuple->X = new_X;
		a_new_tuple->Y = new_Y;
	};
//----------------
friend 
//std::ostream& operator<<(std::ostream& os, const ReflectionProperties& ref);
};
#endif // __SPLINE1D_H_INCLUDED__
