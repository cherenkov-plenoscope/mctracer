//=================================
// include guard
#ifndef __FACTTELESCOPE_H_INCLUDED__
#define __FACTTELESCOPE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "CartesianFrame.h"
#include "OpticalMirrorSphericHexagonal.h"
//=================================
class FactTelescope :public CartesianFrame{
// the fact telescope class stores the geometric information of fact
// and creates a 3D telescope in the world
	std::stringstream telescope_table;

// the inner radius R1 reflector part
	double dbl_focal_length_telescope;
	double MirrorRadius_m;
	std::vector<CartesianFrame*> list_of_mirrors;
public:
//======================================================================
	FactTelescope(double alpha);
//======================================================================	
	void init();
//======================================================================	
	void disp();
//======================================================================	
	std::string get_fact_string();
//======================================================================
	void export_table_of_telescope(std::string telescope_table_file_name);
//======================================================================	
	~FactTelescope();
};
#endif // __FACTTELESCOPE_H_INCLUDED__ 
